modded class GarageMenu extends UIScriptedMenu
{
    private   ButtonWidget      m_BttnParkIn;
    private   ButtonWidget      m_BttnParkOut;
    private   TextListboxWidget m_GarageList;

    private   ImageWidget       m_ParkingStatut;

    private   ItemPreviewWidget m_MainItemPreview;

              bool              m_IsFlag;
              vector            m_Position;
              vector            m_ParkingPos;
              vector            m_ParkingDir;
              vector            m_ParkingOri;
              int               m_LowUID;
              bool              m_PayWithBankAccount;

    private   float             m_transactionTick;
    private   bool              m_canTradeRequest;

    private   ref TStringArray  m_VehiclesName;
    private   ref TStringArray  m_VehiclesListName;

    private   int               m_MoneyAmountOnPlayer;

    private   CarScript         m_CarInPark;

    private   int               m_SelectedVehicle;

    private   EntityAI          m_previewItem;

    void GarageMenu()
    {
        m_MoneyAmountOnPlayer = 0;
        m_CarInPark = NULL;
        m_VehiclesName = new TStringArray;
        m_VehiclesListName = new TStringArray;
    }

    void Show(bool rep)
    {
        if(rep)
        {
        GetGame().GetInput().ChangeGameFocus(1);
        GetGame().GetUIManager().ShowUICursor(true);
        GetGame().GetMission().GetHud().Show(false);
        OnShow();
        }
        else
        {
        GetGame().GetInput().ResetGameFocus();
        GetGame().GetUIManager().ShowUICursor(false);
        GetGame().GetMission().GetHud().Show(true);
        OnHide();
        }
        layoutRoot.Show(rep);
    }

    override void OnShow()
    {
        super.OnShow();
        PPEffects.SetBlurMenu(0.5);
        GetGame().GetInput().ChangeGameFocus(1);
        SetFocus( layoutRoot );
    }

    override void OnHide()
    {
        super.OnHide();
        PPEffects.SetBlurMenu(0);
        GetGame().GetInput().ResetGameFocus();
        if(m_previewItem)
            GetGame().ObjectDelete(m_previewItem);
            Close();
    }

    override Widget Init()
    {
        layoutRoot = GetGame().GetWorkspace().CreateWidgets( "VirtualGarage/gui/VirtualGarageUI.layout" );
        m_BttnParkIn = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "BttnParkIn" ) );
        m_BttnParkOut = ButtonWidget.Cast( layoutRoot.FindAnyWidget( "BttnParkOut" ) );
        m_GarageList = TextListboxWidget.Cast( layoutRoot.FindAnyWidget( "GarageList" ) );
        m_ParkingStatut  = ImageWidget.Cast(layoutRoot.FindAnyWidget( "ParkingStatut" ));
        layoutRoot.Show(false);
        return layoutRoot;
    }

    void UIHandle()
    {
        m_SelectedVehicle = -1;
        m_GarageList.ClearItems();
        m_VehiclesListName.Clear();

        m_CarInPark = GarageHelpers.CheckForCarAtPos(m_ParkingPos);
        if (m_CarInPark)
        {
            #ifdef GMDEBUG
            GetGMLogger().LogInfo("Garage UI m_CarInPark");
            #endif
            m_GarageList.AddItem(m_CarInPark.GetType(), NULL, 0);
            m_GarageList.SetItemColor(0, 0, ARGB(255, 191, 48, 48) );
            m_VehiclesListName.Insert(m_CarInPark.GetType());
        }

        for (int i=0;i<m_VehiclesName.Count();i++)
        {
            #ifdef GMDEBUG
            GetGMLogger().LogInfo(m_VehiclesName[i]);
            #endif
            int pos = i;
            if(m_CarInPark)pos++;
            m_GarageList.AddItem(m_VehiclesName[i], NULL, 0);
            m_GarageList.SetItemColor(pos, 0, ARGB(255, 255, 255, 255) );
            m_VehiclesListName.Insert(m_VehiclesName[i]);
        }

        if (m_ParkingPos == Vector(-1,-1,-1) || m_ParkingPos == Vector(0,0,0) || m_CarInPark)
        {
            m_ParkingStatut.SetColor(ARGB(255,191,48,48));
        }
        else
        {
            m_ParkingStatut.SetColor(ARGB(255,0,255,0));
        }

        if (m_VehiclesListName.Count() > 0)
        {
            m_SelectedVehicle = 0;
            UpdateVehiclePreview(m_VehiclesListName[m_SelectedVehicle]);
        }
    }

    void SetResponseData(TStringArray vehicles, vector parkpos)
    {
        m_VehiclesName.Clear();
        m_VehiclesName = vehicles;
        m_ParkingPos = parkpos;
    }
    
    void SetResponseDataDirOri(vector parkdir, vector parkori)
    {
        m_ParkingDir = parkdir;
        m_ParkingOri = parkori;
    }

    override bool OnClick (Widget w, int x, int y, int button)
    {
        switch(w)
        {
        case m_BttnParkIn: ParkInHandler();
        return true;
        break;

        case m_BttnParkOut: ParkOutHandler();
        return true;
        break;

        case m_GarageList: VehicleListHandler();
        return true;
        break;
        }
        return false;
    }

    void VehicleListHandler()
    {
        m_SelectedVehicle = m_GarageList.GetSelectedRow();
        if(m_SelectedVehicle == -1)return;
        UpdateVehiclePreview(m_VehiclesListName[m_SelectedVehicle]);
    }

    void UpdateVehiclePreview(string itemType)
    {
        string tempstr = itemType;
        tempstr.ToLower();
        if ( !m_MainItemPreview )
        {
            Widget preview_frame = layoutRoot.FindAnyWidget("VehiclePreview");

            if ( preview_frame )
            {
                float width;
                float height;
                preview_frame.GetSize(width, height);

                m_MainItemPreview = ItemPreviewWidget.Cast( GetGame().GetWorkspace().CreateWidget(ItemPreviewWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, ARGB(255, 255, 255, 255), 10, preview_frame) );
            }
        }

        if ( m_previewItem )
            GetGame().ObjectDelete( m_previewItem );

        m_previewItem = EntityAI.Cast(GetGame().CreateObject( itemType, "0 0 0", true, false, true ));
        if(!m_previewItem)return;

        m_MainItemPreview.SetItem(m_previewItem);
        m_MainItemPreview.SetModelPosition( Vector( 0, 0, 0.5 ) );
        m_MainItemPreview.SetModelOrientation( vector.Zero );
        m_MainItemPreview.SetView( m_previewItem.GetViewIndex() );
    }

    void ParkInHandler()
    {
        if(!m_CarInPark)return;
        if(GetGarageConfig().VehicleMustHaveLock)
        {
            if(!GarageHelpers.CanParkVehicle(m_CarInPark))return;
        }
        if(m_VehiclesName.Count() + 1 > GetGarageConfig().MaxVehicleStored)
        {
            NotificationSystem.AddNotificationExtended( 2, "Garage", GetGarageConfig().MaxVehicleStoredReached, "Garage/image/CarLogo.paa" );
            return;
        }

        if(!m_canTradeRequest)return;
        GetRPCManager().SendRPC("Garage", "ParkInRequest",  new Param3<int, CarScript, vector>(m_LowUID, m_CarInPark,m_ParkingPos), true, NULL);
        m_canTradeRequest = false;
    }

    void ParkOutHandler()
    {
        if(m_CarInPark)
        {
        NotificationSystem.AddNotificationExtended( 2, "Garage", GetGarageConfig().ParkingNotAvailable, "Garage/image/CarLogo.paa" );
        //vehicle is blocking garage
        return;
        }
        if(!m_canTradeRequest)return;

        GetRPCManager().SendRPC("Garage", "ParkOutRequest",  new Param6<int, int, string, vector, vector, vector>(m_LowUID,m_SelectedVehicle,m_VehiclesName[m_SelectedVehicle],m_ParkingPos, m_ParkingDir, m_ParkingOri), true, NULL);
        m_canTradeRequest = false;
    }
};
