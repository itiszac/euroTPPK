#include "EuroTPPK.h"
#include "ArrayEX.h"

//funcs
bool HostPlayer = false;
bool TakeNextTP = false;
bool CampOn = false;
bool CampUnitOn = false;
bool MoveBox = false;
bool NPCisSeleceted = false;
DWORD NPCID = 0;
//DWORD GlobalNPCID=0;
POINT Camp;
POINT aPath[255];
POINT WalkPath[255];
DWORD dwCount;
int telex = 0;
DWORD QuestID;
POINT Safed;

bool PathTo(long x, long y) {
	CCollisionMap g_collisionMap;
	if (!g_collisionMap.CreateMap(GetPlayerArea()))
		return false;

	POINT ptStart = GetPosition();
	POINT ptEnd = { x,y };

	if (!g_collisionMap.IsValidAbsLocation(ptStart.x, ptStart.y)) {
		return false;
	}
	if (!g_collisionMap.IsValidAbsLocation(ptEnd.x, ptEnd.y)) {
		return false;
	}

	g_collisionMap.AbsToRelative(ptStart);
	g_collisionMap.AbsToRelative(ptEnd);
	WordMatrix matrix;
	if (!g_collisionMap.CopyMapData(matrix))
		return false;

	g_collisionMap.MakeBlank(matrix, ptStart);
	g_collisionMap.MakeBlank(matrix, ptEnd);
	CTeleportPath tf(matrix.GetData(), matrix.GetCX(), matrix.GetCY());
	dwCount = tf.FindTeleportPath(ptStart, ptEnd, aPath, 255);

	if (dwCount == 0)
		return false;

	else {
		for (DWORD i = 0; i < dwCount; i++) {
			g_collisionMap.RelativeToAbs(aPath[i]);
		}

		AutoTele = true;
		CurrentDW = 1;
		CreateThread(0, 0, TeleportThread, 0, 0, 0);
		//CreateThread(0,0,TeleportMe,0,0,0);
	}
}

void __fastcall KeyHook(BYTE keycode, BYTE repeat) {
	/*if (keycode == vK_AimAndAttack) {
		if (Delay >= v_AIMCYCLE) {
			if (GetDistance(GetPlayerX(GetPlayerID()), GetPlayerY(GetPlayerID()), GetPlayerX(Auto.GetPlayerID()), GetPlayerY(Auto.GetPlayerID())) < 300)
				AttackTo(FindUnit(Auto.GetPlayerID(), 0));
			Delay = 0;
		}
	}*/
	if (repeat) return;

	/*if (keycode == vK_AutoPK) {
		if (Auto.IsPlayerFriend())
			return;
		AutoPK = true;
	}*/

	if (keycode == vK_SilentExit) {
		BYTE Silent[] = { 0x3C,0xDD01,0x80,0x00,0x00,0x00,0x00 };//   Set illegal skill (Button)};
		SendGAMEPacket(Silent, sizeof(Silent));
	}

	if (keycode == vK_NormalExit)
	{
		D2CLIENT_ExitGame();
	}

	//if( keycode == vK_WalkPlayer) {
	//    char Buffer[200];
	//    if(FindUnit(Auto.GetPlayerID(),0)!=NULL) {
	//        Tele(GetPlayerX(Auto.GetPlayerID()),GetPlayerY(Auto.GetPlayerID()));
	//        sprintf(Buffer,"::ÿc  Telewalking on %s at (%d %d)",GetNameByID(Auto.GetPlayerID()),GetPlayerX(Auto.GetPlayerID()),GetPlayerY(Auto.GetPlayerID()));
	//        PrintMessage(Buffer,3);
	//        }
	//    if(FindUnit(Auto.GetPlayerID(),0)==NULL) {
	//    sprintf(Buffer,"::ÿc  Unable to locate %s",GetNameByID(Auto.GetPlayerID()));
	//    PrintMessage(Buffer,1);
	//        }
	//    }
	//if( keycode == vK_WalkAndHostile) {
	//    char Buffer[200];
	//    if(FindUnit(Auto.GetPlayerID(),0)!=NULL) {
	//        if(IsTownLevel(GetPlayerArea()))
	//			if(!Auto.IsPlayerFriend()) {
	//	        HostPlayer=true;
	//            Hostile(Auto.GetPlayerID());
	//		    Tele(GetPlayerX(Auto.GetPlayerID()),GetPlayerY(Auto.GetPlayerID()));
	//            sprintf(Buffer,"::ÿc  Hostiling and Telewalking  on %s at (%d %d)",GetNameByID(Auto.GetPlayerID()),GetPlayerX(Auto.GetPlayerID()),GetPlayerY(Auto.GetPlayerID()));
	//            PrintMessage(Buffer,3);
	//            }
	//        if(!IsTownLevel(GetPlayerArea())){
	//            HostPlayer=false;
	//		    Tele(GetPlayerX(Auto.GetPlayerID()),GetPlayerY(Auto.GetPlayerID()));
	//			sprintf(Buffer,"::ÿc  Telewalking  on %s at (%d %d)",GetNameByID(Auto.GetPlayerID()),GetPlayerX(Auto.GetPlayerID()),GetPlayerY(Auto.GetPlayerID()));
	//            PrintMessage(Buffer,3);
	//            }
	//        if(FindUnit(Auto.GetPlayerID(),0)==NULL) {
	//            sprintf(Buffer,"::ÿc  Unable to locate %s",GetNameByID(Auto.GetPlayerID()));
	//            PrintMessage(Buffer,1);
	//            }
	//        }
	//    }
	//if (keycode == vK_FlashTarget) {
	//	char Buffer[200];
	//	if (Auto.IsPlayerFriend()) {
	//		return;
	//	}
	//	if (Auto.GetPlayerID() == GetPlayerID()) {
	//		return;
	//	}
	//	else if (!Flash) {
	//		sprintf(Buffer, "::ÿc  Flashing %s", GetNameByID(Auto.GetPlayerID()));
	//		// PrintMessage(Buffer,3);
	//		char lpszText[100];
	//		sprintf(lpszText, "Flashing %s", GetNameByID(Auto.GetPlayerID()));
	//		if (lpszText && lpszText[0])
	//		{
	//			UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
	//			OverheadMsg* pMsg = D2COMMON_GenerateOverheadMsg(NULL, lpszText, *p_D2CLIENT_OverheadTrigger);
	//			if (pMsg)
	//			{
	//				D2COMMON_FixOverheadMsg(pMsg, NULL);
	//				pUnit->pOMsg = pMsg;
	//			}
	//		}

	//		Flash = true;
	//	}
	//	else if (Flash) {
	//		sprintf(Buffer, "::ÿc  Stopped Flashing %s", GetNameByID(Auto.GetPlayerID()));
	//		//PrintMessage(Buffer,1);
	//		char lpszText[100];
	//		sprintf(lpszText, "Stopped Flashing %s", GetNameByID(Auto.GetPlayerID()));
	//		if (lpszText && lpszText[0])
	//		{
	//			UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
	//			OverheadMsg* pMsg = D2COMMON_GenerateOverheadMsg(NULL, lpszText, *p_D2CLIENT_OverheadTrigger);
	//			if (pMsg)
	//			{
	//				D2COMMON_FixOverheadMsg(pMsg, NULL);
	//				pUnit->pOMsg = pMsg;
	//			}
	//		}
	//		Flash = false;
	//	}
	//}

	if (keycode == vK_FastTP) {
		if (IsTownLevel(GetPlayerArea())) return;
		TakeNextTP = true;
		HostPlayer = false;
		if (!MakeTP())
			TakeNextTP = false;
		else  return;
	}

	if (keycode == vK_TargetBox) {
		if (TargetList)
			TargetList = false;
		else
			TargetList = true; currentTarget = true;
	}

	//Select Next Target
	if (keycode == vK_NextTarget) {
		Auto.GetNextPlayer();
		if (Auto.GetPlayerID() == GetPlayerID())
			Auto.GetNextPlayer();
	}

	BYTE NPCFollow[17] = { 0x59,0x01,0,0,0,0x00,0x00,0x00,0x00,0,0,0x00,0x00 ,0,0,0x00,0x00 };

	if (keycode == vK_NPCAdd) {
		//ReadConfigFile();
		char Buffer[100];
		// CreateThread(0,0,PKMessage,0,0,0);
		//sprintf(Buffer,"Life is %d",v_AutoDrinkLife);
		//PrintMessage(Buffer,8);

		DWORD ITEM = *(DWORD*)0x6FBB5540;
		//Auto.SelectPlayer(ITEM);

		if (!NPCisSeleceted) {
			if (ITEM != 0) {
				char temp[1000];
				sprintf(temp, "::ÿc		 ID: %X ", ITEM);
				//PrintMessage(temp,3);
				char lpszText[100];
				sprintf(lpszText, "Set NPC ID %X", ITEM);
				if (lpszText && lpszText[0])
				{
					UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
					OverheadMsg* pMsg = D2COMMON_GenerateOverheadMsg(NULL, lpszText, *p_D2CLIENT_OverheadTrigger);
					if (pMsg)
					{
						D2COMMON_FixOverheadMsg(pMsg, NULL);
						pUnit->pOMsg = pMsg;
					}
				}
				NPCisSeleceted = true;
				NPCID = ITEM;
				return;
			}
		}
		if (NPCisSeleceted) {
			if (ITEM != NPCID) { // new npc
				char temp[1000];
				sprintf(temp, "::ÿc  Changed NPC ID: %X ", ITEM);
				//PrintMessage(temp,3);
				char lpszText[100];
				sprintf(lpszText, "Changed NPC ID: %X", ITEM);
				if (lpszText && lpszText[0])
				{
					UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
					OverheadMsg* pMsg = D2COMMON_GenerateOverheadMsg(NULL, lpszText, *p_D2CLIENT_OverheadTrigger);
					if (pMsg)
					{
						D2COMMON_FixOverheadMsg(pMsg, NULL);
						pUnit->pOMsg = pMsg;
					}
				}
				NPCisSeleceted = true;
				NPCID = ITEM;
				return;
			}
		}
		return;
	}

	if (keycode == cfg.TestCommands) {
		//ReadConfigFile();
		if (show)
			show = false;
		//	else show=true;

	//CreateThread(0,0,PMThread,0,0,0);

			/*char Buffer[100];
				sprintf(Buffer,"i found %d monsters here",EnchList->GetItemCount());
			PrintMessage(Buffer,8);*/
			/*
				char Buffer[20];
				wchar_t* cText;
				cText = D2LANG_GetLocaleText(0xF9A);

		*/

		//	sprintf(Buffer,"%sc8 Test with GetLocaleText",cText);
			//PrintMessage(Buffer,0);
		//	sprintf(Buffer,"%sc8 Test with GetLocaleText");
		//	PrintMessage(Buffer,0);

		//	PrintMessage("фc8 Test Without GetLocaleText",0);

			//totalNotifiersN
			//GameEndPatch();

			/*CHAR *lpszText;
			lpszText="Rand";
			Precasts=true;
			if(lpszText && lpszText[0])
					{
							UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
							OverheadMsg* pMsg = D2COMMON_GenerateOverheadMsg(NULL, lpszText, *p_D2CLIENT_OverheadTrigger);
							if(pMsg)
							{
									D2COMMON_FixOverheadMsg(pMsg, NULL);
									pUnit->pOMsg = pMsg;
							}
					}*/

					/*
					DWORD stuff = *(DWORD*)0x6FBB5540;
				char temp[1000];
				temp[0] = 0;
				sprintf(temp+strlen(temp), "STUFF : %X ",stuff);
				PrintMessage(temp,0);

				sprintf(temp,"Name : %s",GetNameByID(stuff));
				PrintMessage(temp,0);

					/*
			DWORD NPCID;
			if(D2CLIENT_GetPlayerUnit() && D2CLIENT_GetPlayerUnit()->pAct)
				{
					for(Room1* pRoom = D2CLIENT_GetPlayerUnit()->pAct->pRoom1; pRoom; pRoom = pRoom->pRoomNext)
					{
			for(UnitAny* pUnit = pRoom->pUnitFirst; pUnit; pUnit = pUnit->pListNext)
						{
							if(pUnit->dwType == UNIT_TYPE_NPC)
								if(pUnit->dwTxtFileNo = 459)
							NPCID =   pUnit->dwUnitId;
				UnitAny pMe;
				 memcpy(&NPCFollow[5],&pUnit->dwUnitId,sizeof(DWORD));
				 memcpy(&NPCFollow[9],&pMe.wX,sizeof(WORD));
				memcpy(&NPCFollow[13],&pMe.wY,sizeof(WORD));
				//SendGAMEPacket(NPCFollow,17);
				char temp1[1000];
				sprintf(temp1, "dwUnitID : %X ",NPCID);
					PrintMessage(temp,0);
				return;
				}
					}
			}*/

			/*
	if(FindUnit(GetIDByName(lpPlayerName),0)!=NULL) {
						UnitAny* pUnit = (UnitAny*)FindUnit(GetIDByName(lpPlayerName),UNIT_TYPE_PLAYER);
						if(D2CLIENT_GetPlayerUnit())
							if(GetSkill(D2S_ENCHANT))
								SetSkill(D2S_ENCHANT, bLeft);
								BYTE *aPacket = new BYTE[9];
								aPacket[0] = bLeft ? 0x06 : 0x0D;
								*(DWORD*)&aPacket[1] = pUnit->dwType;
								*(DWORD*)&aPacket[5] = pUnit->dwUnitId;
								D2NET_SendPacket(9, 1, aPacket);
								delete aPacket;
								return TRUE;
				}

	*/

	//	TestLol();

			/*int i;
			int c=20;*/
			//		char Buffer[200];
					/*
					for(int i=0;i<c;i++) {
					sprintf(Buffer,"Color %d",i);
					PrintMessage(Buffer,i);
					}*/

					/*		UnitAny *pUnit = D2CLIENT_GetPlayerUnit();

							for (int i = 0; i < 20;i ++) {
						DWORD stuff = *(DWORD*)0x6FF316F0;
						char temp[1000];
						temp[0] = 0;
						sprintf(temp+strlen(temp), "%X ", *(DWORD *)(stuff+i*4)) ;
							PrintMessage(temp,0);
							}*/

							//
							//for(RosterUnit* pUnit = (RosterUnit*)*(DWORD*)PlayerRoster; pUnit; pUnit = pUnit->pNext)
							//	if(pUnit->dwUnitId == dwUnitId)
							//		PrintMessage("LOL",1);
						//	sprintf(Buffer,"PartyID: %d",D2CLIENT_PlayerUnitList()->;
						//    PrintMessage(Buffer,1);
						  //  sprintf(Buffer,"Level: %s",GetD2LevelName(pUnit->pPath->pRoom1->pRoom2->pLevel->dwLevelNo));
						   // PrintMessage(Buffer,1);
						  //  sprintf(Buffer,"Act: %d",D2CLIENT_GetPlayerUnit()->dwAct+1);
						  //  PrintMessage(Buffer,1);
						  //  sprintf(Buffer,"dwType: %d",D2CLIENT_GetPlayerUnit()->dwType);
						   // PrintMessage(Buffer,1);
							//GameStructInfo* pGame=GetGameInfo();
						   // sprintf(Buffer,"/w *pUnit .say .:. New Game .:. %s//%s",pGame->szGameName,pGame->szGamePassword);
							//BNCLIENT_SendBNMessage(Buffer);
						  //  GiveFinger();
						//	pk();
							//PlayerStats();
						//PrintScreen();

							//SendMessageA((HWND)D2GFX_GetHwnd(),VK_SNAPSHOT,100,100);
							//PrintScreen(D2CLIENT_MouseX,D2CLIENT_MouseY);

							//SendMessage((HWND)D2GFX_GetHwnd(),WM_RBUTTONUP,0,MAKELONG(X,Y));
							// SendMessage (nHwnd, WM_SYSCOMMAND, SC_MINIMIZE, nlParam);
							//PrintMessage("Capture done",1);
							/*

					SendMessage ( nHwnd, nMsg, nwParam, nlParam );
						Parameter Description
					nHwnd Specifies the handle that identifies the window to receive the message.
					nMsg Specifies the message to send to the window(s).
					nwParam Specifies additional message information.
					nlParam Specifies additional message information.

							*/

							//SendMessage(D2GFX_GetHwnd(), WM_COMMAND, VK_SNAPSHOT, 1);
								//	keybd_event (VK_SNAPSHOT, 1, 0, 0);
							/*D2WIN_TakeScreenshot();
							PrintMessage("Capture done",1);*/
							//		SS();
		BYTE aPacket[10];
		aPacket[0] = 0x1D;
		aPacket[1] = 0x2D;
		aPacket[2] = 0x0A;
		aPacket[3] = 0xEF;
		aPacket[4] = 0x84;
		aPacket[5] = 0xC5;
		aPacket[6] = 0x27;
		aPacket[7] = 0x00;
		aPacket[8] = 0x00;
		aPacket[9] = 0x9C;
		aPacket[10] = 0x19;
		//  D2NET_ReceivePacket(aPacket, sizeof(aPacket));
		  //sprintf(Buffer,"dwMode: %d",D2CLIENT_GetPlayerUnit()->dwMode);
		  //    PrintMessage(Buffer,1);
	}

	//if (keycode == vK_TPPKGo) {
	//	if (Auto.IsPlayerFriend()) {
	//		//PrintMessage("::ÿc  Abort TPPK, You are not allowed to TPPK Mates!",1);
	//		char lpszText[100];
	//		sprintf(lpszText, "Abort TPPK!");
	//		if (lpszText && lpszText[0])
	//		{
	//			UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
	//			OverheadMsg* pMsg = D2COMMON_GenerateOverheadMsg(NULL, lpszText, *p_D2CLIENT_OverheadTrigger);
	//			if (pMsg)
	//			{
	//				D2COMMON_FixOverheadMsg(pMsg, NULL);
	//				pUnit->pOMsg = pMsg;
	//			}
	//		}
	//		return;
	//	}
	//	/*	if(PlayerFriendList->GetItemCount()==0) {
	//			RemovePatchs();
	//			}*/
	//	if (MakeTP()) {
	//		TakeNextTP = true;
	//		HostPlayer = true;
	//		//	 PrintMessage("ÿc3::ÿcOmg you evil man! TPPK'ing!",0);
	//		//	 PrintMessage("::ÿc  Casting TP",3);
	//	}
	//	else return;
	//	// PrintMessage("::ÿc  Abort TPPK, You are in town!",1);
	//}
	//// AA
	if (keycode == vK_AutoAimToggle) {
		if (AA) {
			AA = false;
			char lpszText[100];
			sprintf(lpszText, "Auto-Aim turned OFF");
			if (lpszText && lpszText[0])
			{
				UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
				OverheadMsg* pMsg = D2COMMON_GenerateOverheadMsg(NULL, lpszText, *p_D2CLIENT_OverheadTrigger);
				if (pMsg)
				{
					D2COMMON_FixOverheadMsg(pMsg, NULL);
					pUnit->pOMsg = pMsg;
				}
			}
		}
		else {
			AA = true;
			char lpszText[100];
			sprintf(lpszText, "Auto-Aim turned ON");
			if (lpszText && lpszText[0])
			{
				UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
				OverheadMsg* pMsg = D2COMMON_GenerateOverheadMsg(NULL, lpszText, *p_D2CLIENT_OverheadTrigger);
				if (pMsg)
				{
					D2COMMON_FixOverheadMsg(pMsg, NULL);
					pUnit->pOMsg = pMsg;
				}
			}
		}
	}

	if (keycode == vK_AimKeyToggle)
		if (LeftAA) {
			char lpszText[100];
			sprintf(lpszText, "Aim Button Switched to Right!");
			if (lpszText && lpszText[0])
			{
				UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
				OverheadMsg* pMsg = D2COMMON_GenerateOverheadMsg(NULL, lpszText, *p_D2CLIENT_OverheadTrigger);
				if (pMsg)
				{
					D2COMMON_FixOverheadMsg(pMsg, NULL);
					pUnit->pOMsg = pMsg;
				}
			}
			LeftAA = 0;
		}
		else {
			char lpszText[100];
			sprintf(lpszText, "Aim Button Switched to Left!");
			if (lpszText && lpszText[0])
			{
				UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
				OverheadMsg* pMsg = D2COMMON_GenerateOverheadMsg(NULL, lpszText, *p_D2CLIENT_OverheadTrigger);
				if (pMsg)
				{
					D2COMMON_FixOverheadMsg(pMsg, NULL);
					pUnit->pOMsg = pMsg;
				}
			}
			LeftAA = 1;
		}

	if (keycode == vK_Camp) {
		if (CampUnitOn) {
			//PrintMessage("::ÿc  Switching from CampUnit, to normal Camp!",1);
			char lpszText[100];
			sprintf(lpszText, "CampUnit -> Camp");
			if (lpszText && lpszText[0])
			{
				UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
				OverheadMsg* pMsg = D2COMMON_GenerateOverheadMsg(NULL, lpszText, *p_D2CLIENT_OverheadTrigger);
				if (pMsg)
				{
					D2COMMON_FixOverheadMsg(pMsg, NULL);
					pUnit->pOMsg = pMsg;
				}
			}

			CampUnitOn = false;
		}

		if (IsTownLevel(GetPlayerArea())) {
			/*PrintMessage("::ÿc  Abort Camp, You are in town!",1);*/
			return;
		}

		if (!CampOn) {
			GetD2MousePos(Camp);
			char Buf[100];
			//sprintf(Buf,"::ÿc  Camping and pressing Right at (%d %d)",Camp.x,Camp.y);
			//PrintMessage(Buf,3);
			char lpszText[100];
			sprintf(lpszText, "Camping at (%d %d)", Camp.x, Camp.y);
			if (lpszText && lpszText[0])
			{
				UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
				OverheadMsg* pMsg = D2COMMON_GenerateOverheadMsg(NULL, lpszText, *p_D2CLIENT_OverheadTrigger);
				if (pMsg)
				{
					D2COMMON_FixOverheadMsg(pMsg, NULL);
					pUnit->pOMsg = pMsg;
				}
			}
			CampOn = true;
		}
		else {
			//	PrintMessage("::ÿc  Abort Camp",1);
			CampOn = false;
			HoldRightUp(Camp.x, Camp.y);
		}
	}

	//if (keycode == vK_CampTarget) {
	//	if (CampOn) {
	//		return;
	//	}
	//	if (IsTownLevel(GetPlayerArea())) {
	//		//PrintMessage("::ÿc  Abort CampUnit, You are in town!",1);
	//		return;
	//	}
	//	if (Auto.IsPlayerFriend()) {
	//		//PrintMessage("::ÿc  Abort CampUnit, You are not allowed to TPPK Mates!",1);
	//		return;
	//	}
	//	/*	if(Auto.GetPlayerID()==GetPlayerID()) {
	//			PrintMessage("ÿc1::ÿc  Abort CampUnit, Dont camp yourself!",0);
	//		}*/
	//	if (!CampUnitOn) {
	//		GetD2MousePos(Camp);
	//		char Buf[100];
	//		DWORD	TargetID = Auto.GetPlayerID();
	//		sprintf(Buf, "::ÿc  Camping and pressing Right at (%d %d) Target: (%s)", Camp.x, Camp.y, Auto.GetPlayerNAME());
	//		//	PrintMessage(Buf,3);
	//		char lpszText[100];
	//		sprintf(lpszText, "Camping (%s)", Auto.GetPlayerNAME());
	//		if (lpszText && lpszText[0])
	//		{
	//			UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
	//			OverheadMsg* pMsg = D2COMMON_GenerateOverheadMsg(NULL, lpszText, *p_D2CLIENT_OverheadTrigger);
	//			if (pMsg)
	//			{
	//				D2COMMON_FixOverheadMsg(pMsg, NULL);
	//				pUnit->pOMsg = pMsg;
	//			}
	//		}
	//		CampUnitOn = true;
	//	}
	//	else {
	//		//	PrintMessage("::ÿc  Abort Camp",1);
	//		CampUnitOn = false;
	//		HoldRightUp(Camp.x, Camp.y);
	//	}
	//}

	if (keycode == vK_DrawPlayerHooks) {
		if (ConfigPlayerInfo)
			ConfigPlayerInfo = false;
		else ConfigPlayerInfo = true;
	}

	// Wps
	if (keycode == vK_TeleWP) {
		if (!IsTownLevel(GetPlayerArea()))
			if (GetSkill(0x36) != NULL)
				if (GlobalVectors.WPPos.x != NULL && GlobalVectors.WPPos.y) {
					QuestID = 1;
					PathTo(GlobalVectors.WPPos.x, GlobalVectors.WPPos.y);
				}
				else PrintMessage("::ÿc  No Vaild WP Vector!", 1);
			else PrintMessage("::ÿc  No Teleport Skill found!", 1);
		else PrintMessage("::ÿc  Can't Path in town!", 1);
	}
	// Quest
	if (keycode == vK_TeleNext) {
		if (!IsTownLevel(GetPlayerArea()))
			if (GetSkill(0x36) != NULL)
				if (GlobalVectors.QuestPos.x != NULL && GlobalVectors.QuestPos.y) {
					QuestID = 2;
					PathTo(GlobalVectors.QuestPos.x, GlobalVectors.QuestPos.y);
				}
				else PrintMessage("::ÿc  No Vaild QuestPos Vector!", 1);
			else PrintMessage("::ÿc  No Teleport Skill found!", 1);
		else PrintMessage("::ÿc  Can't Path in town!", 1);
	}
	// Other
	if (keycode == vK_TelePre) {
		if (!IsTownLevel(GetPlayerArea()))
			if (GetSkill(0x36) != NULL)
				if (GlobalVectors.OtherPos.x != NULL && GlobalVectors.OtherPos.y) {
					QuestID = 3;
					PathTo(GlobalVectors.OtherPos.x, GlobalVectors.OtherPos.y);
				}
				else PrintMessage("::ÿc  No Vaild Others Vector!", 1);
			else PrintMessage("::ÿc  No Teleport Skill found!", 1);
		else PrintMessage("::ÿc  Can't Path in town!", 1);
	}

	if (keycode == vK_EnchbotToggle) {
		char Buffer[100];
		if (GetSkill(D2S_ENCHANT))
			if (!EnchBot) {
				EnchBot = true;
				sprintf(Buffer, "EuTPPK Enchant bot Activated! Commands: <%s> ,<%s> ,<%s>", v_EnchantMe, v_EnchantMerc, v_EnchantAll);
				PrintChat(Buffer);
				CreateThread(0, 0, AutoPartyThread, 0, 0, 0);
			}
			else {
				EnchBot = false;
				PrintChat("Enchbot Deactivated!");
				TerminateThread(AutoPartyThread, 0);
			}
	}

	/*if (keycode == vK_ItemViewer) {
		char Buffer[200];
		if (FindUnit(Auto.GetPlayerID(), 0) != NULL) {
			DrawPlayerInventory = true;  InstallPatches(); ToggleInventory(true);
		}
		else {
			sprintf(Buffer, "::ÿc  Can't find %s", GetNameByID(Auto.GetPlayerID()));
			char lpszText[100];
			sprintf(lpszText, "Can't find %s", GetNameByID(Auto.GetPlayerID()));
			if (lpszText && lpszText[0])
			{
				UnitAny* pUnit = D2CLIENT_GetPlayerUnit();
				OverheadMsg* pMsg = D2COMMON_GenerateOverheadMsg(NULL, lpszText, *p_D2CLIENT_OverheadTrigger);
				if (pMsg)
				{
					D2COMMON_FixOverheadMsg(pMsg, NULL);
					pUnit->pOMsg = pMsg;
				}
			}
		}
	}*/
	if (keycode == vK_SpamButton)
	{
		CreateThread(0, 0, SpamMessage, 0, 0, 0);
	}

	if (keycode == vK_NextSong) {
		NextSong();
	}
	if (keycode == vK_PrevSong) {
		PrevSong();
	}
	if (keycode == vK_StopSong) {
		StopSong();
	}
	if (keycode == vK_PlaySong) {
		PlayPause();
	}
	if (keycode == vK_PrintSong) {
		//PrintChat(AmpControl_GetPlayingTitle());
		//sprintf(Buffer,"%s (%d:%02d/%d:%02d)",GetCurrentWinampTrack(),ct/(1000*60), (ct/1000)%60,mt/(60), (mt)%60)
		PrintChat(GetCurrentWinampTrack());
	}

	if (keycode == vK_OpenStash) {
		if (D2CLIENT_GetUiVar(0x19))
			D2CLIENT_SetUIVar(0x19, 1, 0);
		else {
			BYTE data[2] = { 0x77, 0x10 };
			D2NET_ReceivePacket(data, 2);
		}
	}

	if (keycode == vK_DrawPlayerInfo)
		if (ToggleInfo) {
			ToggleInfo = false;
			TargetInfo = false;
		}
		else { ToggleInfo = true; TargetInfo = false; }

	if (keycode == vK_ItemViewer && Auto.GetPlayerID() != 0 && Auto.GetPlayerID() == GetPlayerID()) {
		if (TargetInfo) {
			ToggleInfo = true;
			TargetInfo = true;
		}
		else {
			ToggleInfo = false;
			TargetInfo = false;
		}
	}
	//end of KeyHook
}
WORD GetCurrentSkill(BOOL bLeft)
{
	UnitAny* Me = D2CLIENT_GetPlayerUnit();
	if (bLeft)
		return Me->pInfo->pLeftSkill->pSkillInfo->wSkillId;
	else
		return Me->pInfo->pRightSkill->pSkillInfo->wSkillId;
}

DWORD WINAPI TeleportMe(LPVOID P) {
	UnitAny* Me = D2CLIENT_GetPlayerUnit();
	int max = 0;
	while (aPath[max].x != NULL && aPath[max].y != NULL)
	{
		max++;
	}
	int level = 0;
	bool bArrived = false;
	int nIterations = 0;
	while (AutoTele) {
		if (GetCurrentSkill(FALSE) != D2S_TELEPORT)
			SetSkill(D2S_TELEPORT, FALSE);

		while (GetCurrentSkill(FALSE) != D2S_TELEPORT)
			Sleep(1);
		BYTE aPacket[5];
		aPacket[0] = 0x0C;
		*(LPWORD)&aPacket[1] = (WORD)aPath[level].x;
		*(LPWORD)&aPacket[3] = (WORD)aPath[level].y;
		D2NET_SendPacket(5, 1, aPacket);

		while (nIterations < 1000)
		{
			if (IsTownLevel(GetPlayerArea()) || TakeNextTP || !D2CLIENT_GetPlayerUnit) {
				AutoTele = false;
				break;

				if (CalculateDistance(Me->pPath->xPos, Me->pPath->yPos, aPath[max].x, aPath[max].y) <= 5)
				{
					bArrived = TRUE;
					break;
				}

				Sleep(1);
				nIterations++;
			}
		}
		return TRUE;
	}
}
DWORD WINAPI TeleportThread(LPVOID P) {
	int max = 0;
	while (aPath[max].x != NULL && aPath[max].y != NULL) {
		max++;
	}

	int Timeouts = 0;
	bool Reached = false;
	int TimeOut = 0;
	int level = 0;
	while (AutoTele) {
		if (IsTownLevel(GetPlayerArea()) || TakeNextTP || !D2CLIENT_GetPlayerUnit) {
			AutoTele = false;
			return 0;
		}
		TimeOut++;
		if (Reached) {
			TeleportTo(aPath[level].x, aPath[level].y);
			Reached = false;
		}
		else if (!Reached && TimeOut > 10) {
			TimeOut = 0;
			TeleportTo(aPath[level].x, aPath[level].y);
			//TeleportToTele(aPath[level].x,aPath[level].y);
			Timeouts++;
		}

		if (GetDistanceSquared(GetPosition().x, GetPosition().y, aPath[level].x, aPath[level].y) < 4 && level != max) {
			Reached = true;
			level++;
			TimeOut = 0;
		}

		if (level == max) {
			AutoTele = false;
			TimeOut = 0;
			level = 0;
			Reached = true;
		}
		Sleep(50);
	}

	AutoTele = false;
	DWORD WQuestID;
	DWORD Type;

	if (QuestID == 1) {
		WQuestID = GlobalVectors.WPID;
		Type = 0x02;
	}
	if (QuestID == 2) {
		WQuestID = GlobalVectors.QuestID;
		Type = 0x05;
	}
	if (QuestID == 3) {
		WQuestID = GlobalVectors.OtherID;
		Type = 0x05;
	}

	Sleep(100);

	if (WQuestID) {
		BYTE EnterPacket[] = { 0x13,0x05,0,0,0, 0x00,0x00,0x00,0x00 };
		*(DWORD*)&EnterPacket[1] = Type;
		*(DWORD*)&EnterPacket[5] = WQuestID;
		SendGAMEPacket(EnterPacket, sizeof(EnterPacket));
	}

	return true;
}