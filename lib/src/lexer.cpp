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

        if (tokens.empty() or tokens.back().type != TokenType::EndOfFile)
        {
            tokens.push_back(consumeToken(TokenType::EndOfFile, {}));
        }

        return tokens;
    }

    Token Lexer::nextToken()
    {
        consumeWhitespaces();

        if (isAtEnd())
        {
            return consumeToken(TokenType::EndOfFile, {});
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
        throw LexerError { "Unexpected character", line, column };
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

        return consumeToken(type, expectedLexeme.size(), literal);
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

        std::string_view literal = source.substr(1, length);
        return consumeToken(TokenType::String, length + 2, literal);
    }

    Token Lexer::readNumberToken()
    {
        try
        {
            size_t length = 0;
            double literal = toDouble(source, length);
            return consumeToken(TokenType::Number, length, literal);
        }
        catch (const std::exception&)
        {
            throw LexerError { "Invalid number", line, column };
        }
    }

    Token Lexer::consumeToken(TokenType type, size_t lexemeLength, const patchjson::Literal& literal)
    {
        Token token {
            .type = type,
            .lexeme = {},
            .literal = literal,
            .position = position,
            .line = line,
            .column = column
        };

        token.lexeme = consume(lexemeLength);
        return token;
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
