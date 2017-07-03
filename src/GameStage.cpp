#include <GameStage.h>

std::string EnumInfo::game_stage_str(GameStage game_stage)
{
    switch ((int) game_stage) {
        case (int)GameStage::PLAYERS_REGISTRATION: return "PLAYERS_REGISTRATION";
        case (int)GameStage::INFRASTRUCTURES_REGISTRATION: return "INFRASTRUCTURES_REGISTRATION";
        case (int)GameStage::PUT_INITIAL_INFRASTRUCTURES: return "PUT_INITIAL_INFRASTRUCTURES";
        case (int)GameStage::STAGE1_DICE: return "STAGE1_DICE";
        case (int)GameStage::STAGE1_DROP_RESOURCES: return "STAGE1_DROP_RESOURCES";
        case (int)GameStage::STAGE1_MOVE_ROBBER: return "STAGE1_MOVE_ROBBER";
        case (int)GameStage::STAGE1_ROBBING: return "STAGE1_ROBBING";
        case (int)GameStage::STAGE2: return "STAGE2";
        case (int)GameStage::STAGE3: return "STAGE3";
        case (int)GameStage::FINAL_STAGE: return "FINAL_STAGE";
        default: return "UNKNOWN_STAGE";
    }
}
