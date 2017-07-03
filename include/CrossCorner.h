#ifndef CROSSCORNER_H
#define CROSSCORNER_H

#include <string>

enum class CrossCorner {TOP, BOTTOM};

namespace EnumInfo
{
    std::string cross_corner_str(CrossCorner cc);
}

#endif // CROSSCORNER_H
