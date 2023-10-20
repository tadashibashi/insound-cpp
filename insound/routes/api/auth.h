#pragma once
#include <insound/App.h>

#include <insound/routes/Router.h>

namespace Insound {
    class Auth : public Router {
    public:
        Auth();

        crow::Blueprint &config();

        static crow::response post_login(const crow::request &req);
    };


}