modded class GarageCore
{
    override void GarageResponse(CallType type, ParamsReadContext ctx,  PlayerIdentity sender,  Object target)
    {
        if(!GetGame().IsClient())return;

        Param6<ref TStringArray, ref TIntArray ,vector, int, vector, vector> data;

        if (!ctx.Read(data))
            return;

        // GetGame().ChatPlayer("Pos " + data.param3 + " Dir " + data.param5 + " Ori " + data.param6);

        #ifdef GMDEBUG
        GetGMLogger().LogInfo("GarageResponse data:"+data.param1.Count() + " " +  data.param2);
        #endif

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

        m_VehiclesName.Clear();
        m_VehiclesHealth.Clear();

        m_VehiclesName = data.param1;
        m_VehiclesHealth = data.param2;

        if(player && player.m_GarageMenu)
        {
            player.m_GarageMenu.SetResponseData(data.param1,data.param3);
            player.m_GarageMenu.SetResponseDataDirOri(data.param5,data.param6);
            GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(player.m_GarageMenu.UIHandle, 50);

            switch(data.param4)
            {
                case TraderPlusResponse.SELL_SUCCESS:player.PlayTradeSound(true);
                break;

                case TraderPlusResponse.BUY_SUCCESS:player.PlayTradeSound(false);
                break;

                default:
                break;
            }
        }

        #ifdef GMDEBUG
        GetGMLogger().LogInfo("GarageResponse");
        #endif
    }
};
