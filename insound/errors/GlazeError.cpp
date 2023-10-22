#include "GlazeError.h"


namespace Insound {
    GlazeError::GlazeError(
        const glz::parse_error &err,
        const std::string &buffer) : std::runtime_error(
            f("Glaze error, with code {}:\n{}", (int)err.ec,
                glz::format_error(err, buffer))
        ) {}
}
