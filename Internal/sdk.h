#pragma once
#include "stdafx.h"
#include "macro.h"
#include "vec.h"

namespace cod_data
{
	extern QWORD image_base;
	extern DWORD peb_index;

	void init();
}

namespace decrypt
{
	extern DWORD INDEX_ARRAY_OFFSET;
	extern DWORD NAME_ARRAY_OFFSET;
	extern DWORD NAME_LIST_OFFSET;
	extern DWORD CAMERA_OFFSET;
	extern DWORD REFDEF_OFFSET;
	extern DWORD D3D12_SWAP_CHAIN_OFFSET;
	extern DWORD D3D12_CommandQueue_OFFSET;

	namespace _0x150
	{
		extern DWORD ENCRYPT_PTR_OFFSET;
		extern DWORD REVERSED_ADDRESS;
	}

	namespace _0x318
	{
		extern DWORD ENCRYPT_PTR_OFFSET;
		extern DWORD REVERSED_ADDRESS;
	}

	namespace _0x3580
	{
		extern DWORD ENCRYPT_PTR_OFFSET;
		extern DWORD REVERSED_ADDRESS;
		extern DWORD BASE_OFFSET;
		extern DWORD CAMERA_INFO_OFFSET;
		extern DWORD LOCAL_INDEX_OFFSET;
	}

	namespace Visible
	{
		extern DWORD FunctionDisTribute;
		extern DWORD AboutVisibleFunction;
	}
}

namespace cod
{
	enum BONE_INDEX : unsigned long
	{
		//头盔
		BONE_POS_HELMET = 8,

		//头到膀胱
		BONE_POS_HEAD = 7,
		BONE_POS_NECK = 6,
		BONE_POS_CHEST = 5,
		BONE_POS_MID = 4,
		BONE_POS_TUMMY = 3,
		BONE_POS_PELVIS = 2,

		//右脚, 与 BONE_POS_PELVIS 连接
		BONE_POS_RIGHT_FOOT_1 = 21,
		BONE_POS_RIGHT_FOOT_2 = 22,
		BONE_POS_RIGHT_FOOT_3 = 23,
		BONE_POS_RIGHT_FOOT_4 = 24,

		//左脚, 与 BONE_POS_PELVIS 连接
		BONE_POS_LEFT_FOOT_1 = 17,
		BONE_POS_LEFT_FOOT_2 = 18,
		BONE_POS_LEFT_FOOT_3 = 19,
		BONE_POS_LEFT_FOOT_4 = 20,

		//右手, 与 BONE_POS_CHEST 连接
		BONE_POS_LEFT_HAND_1 = 13,
		BONE_POS_LEFT_HAND_2 = 14,
		BONE_POS_LEFT_HAND_3 = 15,
		BONE_POS_LEFT_HAND_4 = 16,

		//左手, 与 BONE_POS_CHEST 连接
		BONE_POS_RIGHT_HAND_1 = 9,
		BONE_POS_RIGHT_HAND_2 = 10,
		BONE_POS_RIGHT_HAND_3 = 11,
		BONE_POS_RIGHT_HAND_4 = 12
	};

	class clientinfo_t
	{
	public:
		unsigned long m_entity_index;  //this+0x0
		BOOL m_is_valid;               //this+0x4
		char pad_0x8[0x4];
		DWORD m_TeamId; //this+0xC
		char pad_0x10[0x7E0];
		QWORD m_PosInfoPtr; //this+0x7F0
	};//size == 0x3580

	class pos_info
	{
	public:
		float get_rotation() const;
	public:
		char pad_0x0[0xC];
		DWORD nIsSoldierEntity;     //this+0xC
		char pad_0x10[0x8];
		void* m_next_member;        //this+0x18
		char pad_0x20[0x18];
		char flags[8];              //this+0x38
		Vector3 m_position;         //this+0x40
		char pad_0x4C[0xC];
		float m_rotation;           //this+0x58
	};

	class entity_t
	{
	public:
		char pad_0x0[0x1D8];
		unsigned short m_entity_index1; //this+0x1D8
		char pad_0x1DA[0x6];
		short m_is_soldier_info; //this+0x1E0
		char pad_0x1E2[0x86];
		unsigned long m_entity_index2; //this+0x268
		char pad_0x26C[0x80];
		BYTE status; //this+0x2EC
	public:
		bool is_alive()
		{
			if ((0xFFED & this->m_is_soldier_info - 1) == 0 && this->m_is_soldier_info != 3)
			{
				if (this->status & 0x1)
					return true;
			}
			return false;
		}
		bool is_valid()
		{
			if ((0xFFED & this->m_is_soldier_info - 1) == 0 && this->m_is_soldier_info != 3)
				return true;
			return false;
		}
	};//size == 0x318

	class refdef_t
	{
	public:
		char pad_0000[8]; //0x0000
		__int32 Width; //0x0008
		__int32 Height; //0x000C
		float FovX; //0x0010
		float FovY; //0x0014
		float Unk; //0x0018
		char pad_001C[8]; //0x001C
		Vector3 Matrix[3]; //0x0024
		char pad_0048[52]; //0x0048
		Vector3 ViewLocationDelayed0; //0x007C
		Vector3 ViewLocationDelayed1; //0x0088
		char pad_0094[2808]; //0x0094
		Vector3 ViewMatrixTest[3]; //0x0B8C
		Vector3 ViewLocation; //0x0BB0
	};

	class control_info
	{
	public:
		Vector2 GetMouseAngle() const;
	public:
		float MouseX;
		float MouseY;
		Vector3 HeadersPos;
	};

	struct name_t
	{
		DWORD m_entity_index;
		char m_szName[36];
	};

	refdef_t* get_camera();
	bool in_game();
	clientinfo_t* get_local_clientinfo(unsigned long* local_index);
	clientinfo_t* get_clientinfo(unsigned long i);
	entity_t* get_entity(unsigned long i);
	bool get_visible_list_base_address(unsigned long i);
	bool is_visible(unsigned long i, float* show_bones);
	std::string get_name(unsigned long i);
	void write_visible_base_address();
	control_info get_control_info();
	bool get_bone_pos(unsigned long i, unsigned long bone_index, Vector3* bone_pos);
	Vector2 get_entity_angle(unsigned long i);
}