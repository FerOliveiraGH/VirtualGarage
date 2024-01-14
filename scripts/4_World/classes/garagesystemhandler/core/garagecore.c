//modded class GarageCore
//{
//    override void GarageResponse(CallType type, ParamsReadContext ctx,  PlayerIdentity sender,  Object target)
//    {
//        if(!GetGame().IsClient())return;
//
//        Param6<ref TStringArray, ref TIntArray ,vector, int, vector, vector> data;
//
//        if (!ctx.Read(data))
//            return;
//
//        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
//
//        m_VehiclesName.Clear();
//        m_VehiclesHealth.Clear();
//
//        m_VehiclesName = data.param1;
//        m_VehiclesHealth = data.param2;
//
//        if(player && player.m_Fabo_VirtualGarageMenu)
//        {
//            player.m_Fabo_VirtualGarageMenu.SetResponseData(data.param1,data.param3);
//            player.m_Fabo_VirtualGarageMenu.SetResponseDataDirOri(data.param5,data.param6);
//            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(player.m_Fabo_VirtualGarageMenu.UIHandle, 50);
//
//            switch(data.param4)
//            {
//                case TraderPlusResponse.SELL_SUCCESS:player.PlayTradeSound(true);
//                break;
//
//                case TraderPlusResponse.BUY_SUCCESS:player.PlayTradeSound(false);
//                break;
//
//                default:
//                break;
//            }
//        }
//    }
//};
