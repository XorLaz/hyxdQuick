#pragma once
#include "colors_widgets.h"
#include <D3D11.h>
#include <VMProtectSDK.h>

namespace settings {

    inline ImVec2 size_menu = ImVec2(640, 410);
    inline ImVec2 size_watermark = ImVec2(479, 50);
    inline ImVec2 size_preview = ImVec2(300, 400);

}

namespace misc {

    inline int tab_count;
    inline int active_tab_count = 0;
    inline float anim_tab = 0;
    inline int tab_width = 85;
    inline float child_add = 0;
    inline float alpha_child = 0;
}

namespace menu {

    inline ImVec4 general_child = ImColor(23, 23, 25);

}

namespace pictures {

    inline ID3D11ShaderResourceView* logo_img = nullptr;
    inline ID3D11ShaderResourceView* aim_img = nullptr;
    inline ID3D11ShaderResourceView* misc_img = nullptr;
    inline ID3D11ShaderResourceView* visual_img = nullptr;
    inline ID3D11ShaderResourceView* world_img = nullptr;
    inline ID3D11ShaderResourceView* settings_img = nullptr;
    inline ID3D11ShaderResourceView* pen_img = nullptr;
    inline ID3D11ShaderResourceView* keyboard_img = nullptr;
    inline ID3D11ShaderResourceView* input_img = nullptr;
    inline ID3D11ShaderResourceView* wat_logo_img = nullptr;
    inline ID3D11ShaderResourceView* fps_img = nullptr;
    inline ID3D11ShaderResourceView* player_img = nullptr;
    inline ID3D11ShaderResourceView* time_img = nullptr;

}

namespace fonts {
    inline ImFont* inter_font;
    inline ImFont* inter_bold_font;
    inline ImFont* inter_bold_font2;
    inline ImFont* inter_bold_font3;
    inline ImFont* inter_bold_font4;
    inline ImFont* inter_font_b;
    inline ImFont* combo_icon_font;
    inline ImFont* weapon_font;
}

namespace features {


    inline int AimRange = 200;
    inline float SmoothSpeed = 8.0F;



    inline int ColorSettingCurrent = 0;
    inline const char* ColorSettingList[]{ "Box", "Line" ,"Distance","Bone"};

    inline int AimPosCurrent = 0;
    inline const char* AimPosList[] = {
    VMProtectDecryptStringA("Head"),
    VMProtectDecryptStringA("Neck"),
    VMProtectDecryptStringA("Left Arm"),
    VMProtectDecryptStringA("Right Arm"),
    VMProtectDecryptStringA("Left Forearm"),
    VMProtectDecryptStringA("Right Forearm"),
    VMProtectDecryptStringA("Left Hand"),
    VMProtectDecryptStringA("Right Hand"),
    VMProtectDecryptStringA("Upper Spine"),
    VMProtectDecryptStringA("Spine"),
    VMProtectDecryptStringA("Left Leg"),
    VMProtectDecryptStringA("Right Leg"),
    VMProtectDecryptStringA("Left Calf"),
    VMProtectDecryptStringA("Right Calf"),
    VMProtectDecryptStringA("Left Foot"),
    VMProtectDecryptStringA("Right Foot")
    };
    
    inline int key, mind = 1;
    inline char input[64] = { "" };



    inline static bool multi_esp[4] = { false ,false ,false ,true };    // ESP ¿ªÆô×´Ì¬
    inline const char* multi_preview[4] = { ("Box"), ("Line"), ("Distance"),  ("Skeleton") };  //  Ñ¡Ïî¿¨
    inline bool AntiScreenshot = true;
    inline bool VerticalSync = false;
    inline bool esp_perview = false;
    inline bool esp_Team = true;
    inline bool Aim = false;
    inline bool DrawRange = false;




    //inline bool watermark = true;
    inline float preview_alpha;
}

