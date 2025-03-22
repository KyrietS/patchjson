#include "lexer.hpp"
#include <cassert>
#include <cctype>
#include <charconv>

namespace
{
    double toDouble(std::string_view view, size_t& length)
    {
        double value = 0;
        const char* first = view.data();
        const char* last = view.data() + view.size();
        auto [ptr, errorCode] = std::from_chars(first, last, value);
        if (errorCode != std::errc {})
        {
            throw std::invalid_argument { "Invalid number" };
        }
        length = ptr - first;
        return value;
    }
}

namespace patchjson
{
    std::vector<Token> Lexer::tokenize()
    {
        std::vector<Token> tokens;
        while (not isAtEnd())
        {
            tokens.push_back(nextToken());
        }

        if (not tokens.empty() and tokens.back().type != TokenType::EndOfFile)
        {
            tokens.push_back(Token { .type = TokenType::EndOfFile, .lexeme = {}, .literal = {}, .position = position });
        }

        return tokens;
    }

    Token Lexer::nextToken()
    {
        consumeWhitespaces();

        if (isAtEnd())
        {
            return Token { .type = TokenType::EndOfFile, .lexeme = {}, .literal = {}, .position = position };
        }

        char firstChar = source.front();
        switch (firstChar)
        {
        case '{':
            return readToken(TokenType::OpenBrace, firstChar);
        case '}':
            return readToken(TokenType::CloseBrace, firstChar);
        case '[':
            return readToken(TokenType::OpenBracket, firstChar);
        case ']':
            return readToken(TokenType::CloseBracket, firstChar);
        case ':':
            return readToken(TokenType::Colon, firstChar);
        case ',':
            return readToken(TokenType::Comma, firstChar);
        case '"':
            return readStringToken();
        case 't':
            return readToken(TokenType::True, "true", true);
        case 'f':
            return readToken(TokenType::False, "false", false);
        case 'n':
            return readToken(TokenType::Null, "null");
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return readNumberToken();
        default:
            break;
        }
        throw LexerError { "Unexpected token", line, column };
    }

    bool Lexer::isAtEnd() const
    {
        return source.size() == 0;
    }

    void Lexer::consumeWhitespaces()
    {
        while (not isAtEnd() and std::isspace(source.front()))
        {
            if (source.front() == '\n')
            {
                line++;
                column = 1;
            }
            else
            {
                column++;
            }
            source.remove_prefix(1);
            position++;
        }
    }

    Token Lexer::readToken(TokenType type, char epxectedChar)
    {
        return readToken(type, std::string_view { &epxectedChar, 1 });
    }


    Token Lexer::readToken(TokenType type, std::string_view expectedLexeme, const patchjson::Literal& literal)
    {
        if (source.size() < expectedLexeme.size())
        {
            throw LexerError { "Unexpected end of input", line, column };
        }

        if (not source.starts_with(expectedLexeme))
        {
            throw LexerError { "Unexpected token", line, column };
        }

        Token token { type };
        token.position = position;
        token.lexeme = consume(expectedLexeme.size());
        token.literal = literal;
        return token;
    }


    Token Lexer::readStringToken()
    {
        if (source.size() < 2)
        {
            throw LexerError { "Unexpected end of input", line, column };
        }

        assert(source.front() == '"');
        size_t length = 0;
        while (source[ 1 + length ] != '"')
        {
            if (std::iscntrl(static_cast<unsigned char>(source[ 1 + length ])))
            {
                throw LexerError { "Unexpected control character inside string", line, column };
            }

            length++;
        }

        Token token { TokenType::String };
        token.position = position;
        token.lexeme = consume(length + 2); // consume opening and closing quotes
        token.literal = token.lexeme.substr(1, token.lexeme.size() - 2); // remove quotes
        return token;
    }

    Token Lexer::readNumberToken()
    {
        try
        {
            Token token { TokenType::Number };
            token.position = position;
            size_t length = 0;
            token.literal = toDouble(source, length);
            token.lexeme = consume(length);
            return token;
        }
        catch (const std::exception&)
        {
            throw LexerError { "Invalid number", position };
        }
    }

    std::string_view Lexer::consume(size_t length)
    {
        assert(source.size() >= length);
        std::string_view lexeme = source.substr(0, length);
        source.remove_prefix(length);
        position += length;
        column += length;
        return lexeme;
    }

}
