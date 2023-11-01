#include "auth.h"

#include <insound/core/mongo/Model.h>
#include <insound/core/MultipartMap.h>
#include <insound/core/password.h>
#include <insound/core/util.h>
#include <insound/server/Server.h>

#include <crow/common.h>
#include <crow/middlewares/cookie_parser.h>

using SameSitePolicy = crow::CookieParser::Cookie::SameSitePolicy;

namespace Insound {
    Auth::Auth() : Router("api/auth") {}

    void Auth::init()
    {
        CROW_BP_ROUTE(bp, "/login/email")
            .methods("POST"_method)
            (Auth::post_login);

        CROW_BP_ROUTE(bp, "/check")
            .methods("GET"_method)
            (Auth::get_check);

        CROW_BP_ROUTE(bp, "/create/email")
            .methods("POST"_method)
            (Auth::post_create);
    }

    void Auth::catchAll(const crow::request &req, crow::response &res)
    {
        IN_LOG("Auth catchall was hit!");
        res.end("Auth catchall!");
    }

    void Auth::get_check(const crow::request &req, crow::response &res)
    {
        auto &cookies = Server::getContext<crow::CookieParser>(req);
        auto &user = Server::getContext<UserAuth>(req).user;

        res.add_header("Content-Type", "application/json");

        auto fingerprint = cookies.get_cookie("fingerprint");

        if (user.isAuthorized() && compare(fingerprint, user.fingerprint))
        {
            res.body = R"({"auth":true})";
        }
        else
        {
            res.body = R"({"auth":false})";
        }

        res.end();
    }

    void Auth::post_login(const crow::request &req, crow::response &res)
    {
        auto &cookies = Server::getContext<crow::CookieParser>(req);

        auto data = MultipartMap::from(req);

        std::string email;
        std::string password;

        try {
            email = data.fields.at("email");
            password = data.fields.at("password");
        }
        catch(...)
        {
            res.code = 400;
            res.end("Missing fields");
            return;
        }

        // Protect from over-querying database
        if (!data.fields["password2"].empty())
        {
            res.code = 200;
            return res.end();
        }

        // check for user
        Mongo::Model<User> UserModel;
        auto userRes = UserModel.findOne({"email", email});
        if (!userRes)
        {
            res.code = 401;
            return res.end(R"({"error":"Could not find a user with that email."})");
        }

        auto &user = userRes.value();

        if (!compare(user.body.password, password))
        {
            res.code = 401;
            return res.end(R"({"error":"Invalid password."})");
        }

        UserToken token;
        token.username = user.body.username;
        token.displayName = user.body.displayName;
        token.email = user.body.email;
        token.type = user.body.type;

        const auto fingerprint = genHexString();
        cookies.set_cookie("fingerprint", fingerprint)
            .httponly()
            .same_site(SameSitePolicy::Strict)
            .max_age(60 * 60 * 24 * 14)
            .path("/");
        token.fingerprint = hash(fingerprint);

        res.body = glz::write_json(token);
        res.code = 200;
        res.end();
    }

    void Auth::post_create(const crow::request &req, crow::response &res)
    {
        auto data = MultipartMap::from(req);

        std::string email;
        std::string password;

        // Protect database from automated requests
        if (!data.fields["username2"].empty())
        {
            res.code = 200;
            return res.end();
        }

        try {
            if (data.fields.at("password") != data.fields.at("password2"))
            {
                res.code = 400;
                return res.end(R"({"error":"Passwords mismatch"})");
                return;
            }

            auto UserModel = Mongo::Model<User>();
            auto user = UserModel.findOne({"email", email});
            if (user)
            {
                res.code = 400;
                return res.end(R"({"error":"User with email account already exists."})");
            }
        }
        catch (...)
        {
            res.code = 400;
            return res.end(R"({"error":"Missing field"})");
        }
    }

} // namespace Insound
