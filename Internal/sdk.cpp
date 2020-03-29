#include "stdafx.h"
#include "sdk.h"

QWORD g_VisibleListBaseAddress;
QWORD g_PreviousVisibleListBaseAddress;

namespace cod_data
{
	QWORD image_base;
	DWORD peb_index;

	void init()
	{
		image_base = (QWORD)GetModuleHandle(NULL);
		peb_index  = (DWORD)(__readgsqword(0x60) >> 0xC) & 0xF;
	}
}

namespace decrypt
{
	DWORD INDEX_ARRAY_OFFSET = 0xDD4B780;
	DWORD NAME_ARRAY_OFFSET = 0x107730E8;
	DWORD NAME_LIST_OFFSET = 0x4C70;
	DWORD CAMERA_OFFSET = 0xCFF6E10;
	DWORD REFDEF_OFFSET = 0x106A4C60;
	DWORD D3D12_SWAP_CHAIN_OFFSET = 0x1201D618;
	DWORD D3D12_CommandQueue_OFFSET = 0x120182A8;
 
	namespace _0x150
	{
		DWORD ENCRYPT_PTR_OFFSET = 0xDD4CB48;
		DWORD REVERSED_ADDRESS = 0x3741397;
	}

	namespace _0x318
	{
		DWORD ENCRYPT_PTR_OFFSET = 0x106A10C8;
		DWORD REVERSED_ADDRESS = 0x3741258;
	}

	namespace _0x3580
	{
		DWORD ENCRYPT_PTR_OFFSET = 0x106A23F8;
		DWORD REVERSED_ADDRESS = 0x37412B8;
		DWORD BASE_OFFSET = 0x96E18;
		DWORD CAMERA_INFO_OFFSET = 0x2128;
		DWORD LOCAL_INDEX_OFFSET = 0x1DE58;
	}

	namespace Visible
	{
		DWORD FunctionDisTribute = 0x1301EFC0;
		DWORD AboutVisibleFunction = 0x2954840;
	}

	QWORD* decrypt_key_for_clientinfo_t()
	{
		static QWORD key[4];

		static bool Empty = true;

		if (Empty)
		{
			QWORD reversedAddr, LastKey;
			reversedAddr = *(QWORD*)(cod_data::image_base + decrypt::_0x3580::REVERSED_ADDRESS);
			LastKey = *(QWORD*)(_byteswap_uint64(reversedAddr) + 0x17);
			key[0] = 0x33A8BCA726B7B983;
			key[1] = 0x50A92D8645F99BB;
			key[2] = LastKey;
			key[3] = 0xA22A4F24EDD27A89;
			Empty = false;
		}

		return (QWORD *)key;
	}

	QWORD* decrypt_key_for_entity()
	{
		static QWORD key[16][4];

		static bool Empty = true;
		if (Empty)
		{
			QWORD reversedAddr, LastKey;
			reversedAddr =*(QWORD*)(cod_data::image_base + decrypt::_0x318::REVERSED_ADDRESS);
			LastKey =*(QWORD*)(_byteswap_uint64(reversedAddr) + 0x1D);

			key[0][0] = LastKey;
			key[0][1] = 0x69CBDD6D2FCFDD7F;
			key[0][2] = 0x906933A7A6454651;
			key[0][3] = 0x576A0C8EF709766F;

			key[1][0] = 0xD99F6CAB2C376BF3;
			key[1][1] = 0x148158D8C7580ABD;
			key[1][2] = LastKey;
			key[1][3] = 0xD57C91B977A23B6D;

			key[2][0] = LastKey;
			key[2][1] = 0xA3C3D120867F9187;
			key[2][2] = 0xB0F113F81623287B;
			key[2][3] = 0xF037D82E18C55A1;

			key[3][0] = 0x9019EA0149459A3F;
			key[3][1] = 0x78CD54516B2A4FA1;
			key[3][2] = LastKey;
			key[3][3] = 0x387AABBFF07D310F;

			key[4][0] = 0xC0718712C6B9E11D;
			key[4][1] = 0x2EB39A5E0974A941;
			key[4][2] = LastKey;
			key[4][3] = 0x85660AE8CDBBA28D;

			key[5][0] = 0xAEB14A674F3306CF;
			key[5][1] = 0x41C47A85E5C188F3;
			key[5][2] = LastKey;
			key[5][3] = 0xE5884703AEBA2E6F;

			key[6][0] = LastKey;
			key[6][1] = 0xAEE38B65C49F056F;
			key[6][2] = 0x157108DFA2FA9501;
			key[6][3] = 0xCD51346A4E8A9AFF;

			key[7][0] = 0xEC56A21C6BAFDC31;
			key[7][1] = LastKey;
			key[7][2] = 0xE8632264C9DDDFC7;
			key[7][3] = 0xA1FD180E465B57C7;

			key[8][0] = 0x3AB48FFF1DC9C449;
			key[8][1] = 0xBF7C9071CE06B19B;
			key[8][2] = 0xE96BE1133DEC06A1;
			key[8][3] = LastKey;

			key[9][0] = 0x85235707D91788E1;
			key[9][1] = 0x102C693153E87933;
			key[9][2] = LastKey;
			key[9][3] = 0x3FE02E494AA4D2DB;

			key[10][0] = 0x4CE3E417110D5817;
			key[10][1] = 0xD063CECE95901849;
			key[10][2] = 0x25814DB616316737;
			key[10][3] = LastKey;

			key[11][0] = LastKey;
			key[11][1] = 0xC068F7CF18FC2F31;
			key[11][2] = 0x196EBBC5929F4C83;
			key[11][3] = 0x71BE721839FA6967;

			key[12][0] = 0x931E2217B131AA7B;
			key[12][1] = 0x7B6C242A4517CACD;
			key[12][2] = LastKey;
			key[12][3] = 0xD32DB622EB32811F;

			key[13][0] = 0x4C0A354A73C1B613;
			key[13][1] = 0xABA3C344D53D8769;
			key[13][2] = 0x7AA0268A1C97EC85;
			key[13][3] = LastKey;

			key[14][0] = LastKey;
			key[14][1] = 0xAAA56DBE6792868B;
			key[14][2] = 0x981EE01DAA06F0AF;
			key[14][3] = 0x835DB861CF84ECCF;

			key[15][0] = LastKey;
			key[15][1] = 0x76D2A7E196713B2F;
			key[15][2] = 0x803485936DDBA421;
			key[15][3] = 0xF095D9C434D2B641;

			Empty = false;
		}

		return (QWORD *)key;
	}

	QWORD* decrypt_key_for_bone()
	{
		static QWORD key[16][4];

		static bool Empty = true;
		if (Empty)
		{
			QWORD reversedAddr, LastKey;

			reversedAddr =*(QWORD*)(cod_data::image_base + decrypt::_0x150::REVERSED_ADDRESS);
			LastKey = *(QWORD*)(_byteswap_uint64(reversedAddr) + 0xD);

			key[0][0] = 0x6365B7F1C4AC539B;
			key[0][1] = LastKey;
			key[0][2] = 0xC072A195BEE46023;
			key[0][3] = 0xDA04D24024AD190D;

			key[1][0] = LastKey;
			key[1][1] = 0x342D260A54C1E583;
			key[1][2] = 0xF0D45C06E78F6FE7;
			key[1][3] = 0xA6176E220F7094B7;

			key[2][0] = 0xBD91E1FB3E40662D;
			key[2][1] = 0xC6F2FB4A871F1883;
			key[2][2] = LastKey;
			key[2][3] = 0x1F5894655E4E6A71;

			key[3][0] = 0x4DF9FC9087988157;
			key[3][1] = 0xBDCC41DE58B4EAD9;
			key[3][2] = LastKey;
			key[3][3] = 0x9E4907CDEF111949;

			key[4][0] = LastKey;
			key[4][1] = 0xAC13D0A1D9204A43;
			key[4][2] = 0x1DF641A0651694FB;
			key[4][3] = 0x33EA52C3AB428AB5;

			key[5][0] = LastKey;
			key[5][1] = 0xB4B8D7DFAF11A791;
			key[5][2] = 0x1C9C4586E2D50EDD;
			key[5][3] = 0x32BE3513B8911CA9;

			key[6][0] = 0x7508C2EAAA75C45;
			key[6][1] = 0xB017FCF21D8A8871;
			key[6][2] = LastKey;
			key[6][3] = 0x4C8746225896F091;

			key[7][0] = 0xA7EA24A30706D491;
			key[7][1] = LastKey;
			key[7][2] = 0x1ABEF7E3A0B92DB7;
			key[7][3] = 0x823A52F672FC8DEB;

			key[8][0] = 0xA037891E2608E399;
			key[8][1] = LastKey;
			key[8][2] = 0x4786F80AD0A25EC5;
			key[8][3] = 0xBB2B0B630A03C095;

			key[9][0] = LastKey;
			key[9][1] = 0x99AFD1794C9C3FD3;
			key[9][2] = 0xE66C47485E7385FD;
			key[9][3] = 0x8C8CEC2A622AC513;

			key[10][0] = LastKey;
			key[10][1] = 0xCDED268065260A1F;
			key[10][2] = 0xF13CBB1D77910011;
			key[10][3] = 0x1FFDE07F8A1B0E35;

			key[11][0] = 0xE5560D0386CCAA23;
			key[11][1] = 0xD38DAB48E3552093;
			key[11][2] = 0xEAA4924255A37DA9;
			key[11][3] = LastKey;

			key[12][0] = LastKey;
			key[12][1] = 0x2C323A8C303B5CE3;
			key[12][2] = 0xF129D199445931ED;
			key[12][3] = 0x57A6274E1D0562EB;

			key[13][0] = 0xC1D6CC2466190809;
			key[13][1] = LastKey;
			key[13][2] = 0xA89351ACFB838AD9;
			key[13][3] = 0xFE33CDEBF197A4F5;

			key[14][0] = 0xAFAB5091C1BD9BD3;
			key[14][1] = LastKey;
			key[14][2] = 0x7B8F77FD2E1F76E5;
			key[14][3] = 0x7DFF38465E220633;

			key[15][0] = 0x13FD8C5545363BBD;
			key[15][1] = 0x7D7D6348F5C900F1;
			key[15][2] = LastKey;
			key[15][3] = 0x2B9B5556F7CE69A9;

			Empty = false;
		}

		return (QWORD *)key;
	}

	QWORD general_obfuscate(QWORD encryptedPtr, QWORD* curr_decrypt_key)
	{
		encryptedPtr = (__int64)encryptedPtr * curr_decrypt_key[0];
		encryptedPtr = encryptedPtr ^ (encryptedPtr >> 0xB);
		encryptedPtr = encryptedPtr ^ (encryptedPtr >> 0x16);
		encryptedPtr = (encryptedPtr >> 0x2C) ^ encryptedPtr;
		encryptedPtr = (__int64)encryptedPtr * curr_decrypt_key[1];
		encryptedPtr = (encryptedPtr >> 0x22) ^ encryptedPtr;
		encryptedPtr = (__int64)encryptedPtr * curr_decrypt_key[2];
		encryptedPtr = encryptedPtr ^ (encryptedPtr >> 0x20);
		encryptedPtr = (__int64)encryptedPtr * curr_decrypt_key[3];
		encryptedPtr = encryptedPtr ^ (encryptedPtr >> 0x21);
		return encryptedPtr;
	}

	QWORD decrypt_clientinfo_t()
	{
		QWORD encryptedPtr = *(QWORD*)(cod_data::image_base + decrypt::_0x3580::ENCRYPT_PTR_OFFSET);

		if (encryptedPtr)
		{
			return general_obfuscate(encryptedPtr, decrypt_key_for_clientinfo_t());
		}

		return 0;
	}


	QWORD decrypt_for_entity()
	{
		QWORD BaseAddress, encryptedPtr;

		BaseAddress = *(QWORD*)(cod_data::image_base + decrypt::_0x318::ENCRYPT_PTR_OFFSET);

		if (BaseAddress)
		{
			encryptedPtr = *(QWORD*)(BaseAddress + 0x10);

			if (encryptedPtr)
			{
				return general_obfuscate(encryptedPtr, &decrypt_key_for_entity()[cod_data::peb_index << 2]);
			}
		}
		return 0;
	}
}

namespace cod
{
	Vector2 control_info::GetMouseAngle() const
	{
		Vector2 angle;
		angle.x = this->MouseX;
		if (this->MouseY > 85.0f)
			angle.y = 360.0f - this->MouseY;
		else
			angle.y = this->MouseY * -1;
		return angle;
	}

	float pos_info::get_rotation() const
	{
		float rotation = m_rotation;

		if (rotation < 0)
			rotation = 360.0f - (rotation * -1);

		rotation += 90.0f;

		if (rotation >= 360.0f)
			rotation = rotation - 360.0f;

		return rotation;
	}

	clientinfo_t* get_clientinfo(unsigned long i)
	{
		QWORD decryptedPtr = decrypt::decrypt_clientinfo_t();

		if (decryptedPtr)
		{
			QWORD BaseAddress = *(QWORD*)(decryptedPtr + decrypt::_0x3580::BASE_OFFSET);

			if (BaseAddress)
			{
				return (clientinfo_t*)(BaseAddress + (i * 0x3580));
			}
		}

		return nullptr;
	}

	clientinfo_t* get_local_clientinfo(unsigned long* local_index)
	{
		QWORD decryptedPtr = decrypt::decrypt_clientinfo_t();

		if (decryptedPtr)
		{
			QWORD BaseAddress = *(QWORD*)(decryptedPtr + decrypt::_0x3580::LOCAL_INDEX_OFFSET);

			if (BaseAddress)
			{
				*local_index = *(DWORD*)(BaseAddress + 0x1DC);
				return get_clientinfo(*local_index);
			}
		}
		*local_index = 0;
		return nullptr;
	}

	entity_t* get_entity(unsigned long i)
	{
		QWORD decryptedPtr = decrypt::decrypt_for_entity();

		if (decryptedPtr)
		{
			entity_t* ptr = (entity_t*)(decryptedPtr + (i * 0x318));

			if (ptr)
			{
				if ((0xFFED & ptr->m_is_soldier_info - 1) == 0 && ptr->m_is_soldier_info != 3)
				{
					return ptr;
				}
			}
		}
		return nullptr;
	}

	refdef_t* get_camera()
	{
		return *(refdef_t **)(cod_data::image_base + decrypt::REFDEF_OFFSET);
	}

	bool get_visible_list_base_address(unsigned long i)
	{//也可以使用改变分辨率的方式刷新, 当刷新之后, 第一个就是了.
		bool is_found = false;

		for (int j = 4000; j >= 0; --j)
		{
			QWORD nIndex = (j + (j << 2)) << 0x6;
			QWORD BaseAddress = cod_data::image_base + decrypt::Visible::FunctionDisTribute + nIndex;
			QWORD cmpFunctionHeader = *(QWORD*)(BaseAddress + 0x90);
			if (cmpFunctionHeader == cod_data::image_base + decrypt::Visible::AboutVisibleFunction)
			{
				QWORD ValidVisibleListBaseAddress = *(QWORD*)(BaseAddress + 0x108);
				QWORD index = ((QWORD)(i + (i * 2))) << 4;

				int VisibleFlags = *(int*)(ValidVisibleListBaseAddress + index + 0xA58);
				
				if (VisibleFlags == 3)
				{
					g_VisibleListBaseAddress = BaseAddress;
					is_found = true;
					break;
				}
			}
		}
		return is_found;
	}

	bool is_visible(unsigned long i, float* show_bones)
	{
		if (g_PreviousVisibleListBaseAddress)
		{
			QWORD VisibleList = *(QWORD*)(g_PreviousVisibleListBaseAddress + 0x108);
			QWORD index = ((QWORD)(i + (i * 2))) << 0x4;
			VisibleList += index;
			if (show_bones)
				*show_bones = *(float*)(VisibleList + 0xA5C);
			int VisibleFlags = *(int*)(VisibleList + 0xA58);
			return VisibleFlags == 3;
		}
		else
		{
			if (show_bones)
			{
				*show_bones = 0.0f;
			}
		}

		return false;
	}

	std::string get_name(unsigned long i)
	{
		QWORD Ptr = *(QWORD*)(cod_data::image_base + decrypt::NAME_ARRAY_OFFSET);
		if (Ptr)
		{
			name_t* name = (name_t*)(Ptr + decrypt::NAME_LIST_OFFSET + ((i + (i * 8)) << 0x4));
			int length = strlen(name->m_szName);
			for (int j = 0; j < length; ++j)
			{
				char ch = name->m_szName[j];
				bool is_english = ch >= 0 && ch <= 127;
				if (!is_english)
					return "Player";
			}
			return name->m_szName;
		}
		return "Player";
	}

	void write_visible_base_address()
	{
		g_PreviousVisibleListBaseAddress = g_VisibleListBaseAddress;
	}

	control_info get_control_info()
	{
		QWORD decryptedPtr = decrypt::decrypt_clientinfo_t();

		if (decryptedPtr)
		{
			control_info ci;

			ci.MouseX = *(float*)(decryptedPtr + 0x195E0);
			ci.MouseY = *(float*)(decryptedPtr + 0x195E4);

			float MouseY  = ci.MouseX;
			ci.MouseX     = ci.MouseY;
			ci.MouseY     = MouseY;
			ci.HeadersPos = *(Vector3*)(decryptedPtr + 0x21A4);

			return ci;
		}

		return control_info();
	}

	bool get_bone_pos(unsigned long i, unsigned long bone_index, Vector3* bone_pos)
	{
		QWORD encryptedPtr, decryptedPtr, bonePtr;

		encryptedPtr = *(QWORD*)(cod_data::image_base + decrypt::_0x150::ENCRYPT_PTR_OFFSET);

		if (encryptedPtr)
		{
			decryptedPtr = decrypt::general_obfuscate(encryptedPtr, &decrypt::decrypt_key_for_bone()[cod_data::peb_index << 2]);

			if (decryptedPtr)
			{
				unsigned short index = *(unsigned short*)(cod_data::image_base + decrypt::INDEX_ARRAY_OFFSET + (i * sizeof(index)));

				if (index != 0)
				{
					bonePtr = *(QWORD*)(decryptedPtr + (index * 0x150) + 0xC0);

					if (bonePtr)
					{
						*bone_pos = *(Vector3*)(bonePtr + (bone_index * 0x20) + 0x10);

						QWORD decryptedPtr = decrypt::decrypt_clientinfo_t();

						if (decryptedPtr)
						{
							Vector3* BasePos = (Vector3*)(decryptedPtr + 0x21A4);
							bone_pos->x += BasePos->x;
							bone_pos->y += BasePos->y;
							bone_pos->z += BasePos->z;
							return true;
						}
					}
				}
			}
		}
		return false;
	}

	Vector2 get_entity_angle(unsigned long i)
	{
		QWORD BaseAddress;
		Vector2 results;

		QWORD decryptedPtr = decrypt::decrypt_clientinfo_t();

		if (decryptedPtr)
		{
			BaseAddress = *(QWORD*)(decryptedPtr + decrypt::_0x3580::BASE_OFFSET);

			if (BaseAddress)
			{
				Vector2* angle = (Vector2*)(BaseAddress + (i * 0x3580) + 0x9B8);
				results.x = angle->y < 0.0f ? (360.0f + angle->y) : angle->y;
				results.y = angle->x <= 0.0f ? std::fabs(angle->x) : 360.0f - angle->x;
				return results;
			}
		}

		return Vector2();
	}
}