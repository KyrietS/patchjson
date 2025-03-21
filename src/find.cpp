#include "find.hpp"
#include "lexer.hpp"
#include <cassert>

namespace patchjson
{
namespace
{
    struct SearchContext
    {
        std::span<const std::string> path;
        Lexer lexer;
        Token nextToken{};
    };

    Token consume(SearchContext& ctx, TokenType type)
    {
        Token token = ctx.nextToken;
        if (token.type != type)
        {
            throw std::runtime_error("Unexpected token");
        }
        ctx.nextToken = ctx.lexer.nextToken();
        return token;
    }

    Token peek(const SearchContext& ctx)
    {
        return ctx.nextToken;
    }

    bool match(const SearchContext& ctx, TokenType type)
    {
        return ctx.nextToken.type == type;
    }

    Token searchValue(SearchContext& ctx);
    Token searchArray(SearchContext& ctx);
    Token getFinalValue(SearchContext& ctx);

    Token searchObject(SearchContext& ctx)
    {
        Token beginObject = consume(ctx, TokenType::OpenBrace);
        if (ctx.path.empty())
        {
            return beginObject;
        }

        while (not match(ctx, TokenType::CloseBrace))
        {
            std::string_view key = std::get<std::string_view>(consume(ctx, TokenType::String).literal.value());
            consume(ctx, TokenType::Colon);
            if (key == ctx.path[0])
            {
                ctx.path = ctx.path.subspan(1);
                return searchValue(ctx);
            }

            if (match(ctx, TokenType::Comma))
            {
                consume(ctx, TokenType::Comma);
            }
        }

        throw std::runtime_error(std::format("Key '{}' not found", ctx.path[0]));
    }

    Token searchValue(SearchContext& ctx)
    {
        if (match(ctx, TokenType::OpenBrace))
        {
            return searchObject(ctx);
        }
        else if (match(ctx, TokenType::OpenBracket))
        {
            return searchArray(ctx);
        }
        else
        {
            return getFinalValue(ctx);
        }
    }

    Token searchArray(SearchContext& ctx)
    {
        Token beginArray = consume(ctx, TokenType::OpenBracket);
        if (ctx.path.empty())
        {
            return beginArray;
        }

        size_t searchIndex = std::stoul(ctx.path[0]);
        size_t index = 0;
        while (not match(ctx, TokenType::CloseBracket))
        {
            if (index == searchIndex)
            {
                ctx.path = ctx.path.subspan(1);
                return searchValue(ctx);
            }
            else
            {
                // consumeValue(ctx);
            }

            if (match(ctx, TokenType::Comma))
            {
                consume(ctx, TokenType::Comma);
            }
            index++;
        }

        throw std::runtime_error(std::format("Index '{}' out of range", index));
    }

    Token getFinalValue(SearchContext& ctx)
    {
        if (not ctx.path.empty())
        {
            throw std::runtime_error(std::format("Key '{}' not found", ctx.path[0]));
        }

        if (match(ctx, TokenType::String))
        {
            return consume(ctx, TokenType::String);
        }
        else if (match(ctx, TokenType::Number))
        {
            return consume(ctx, TokenType::Number);
        }
        else if (match(ctx, TokenType::True))
        {
            return consume(ctx, TokenType::True);
        }
        else if (match(ctx, TokenType::False))
        {
            return consume(ctx, TokenType::False);
        }
        else if (match(ctx, TokenType::Null))
        {
            return consume(ctx, TokenType::Null);
        }
        else
        {
            throw std::runtime_error("Unexpected token");
        }
    }
}

    Token findValue(std::string_view source, const JsonPath& path)
    {
        SearchContext context{
            .path = path,
            .lexer = Lexer{source}
        };
        context.nextToken = context.lexer.nextToken();

        return searchObject(context);
    }
}