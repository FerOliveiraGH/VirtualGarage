class Fabo_VirtualGarageMenu extends UIScriptedMenu
{
    static float lastClickTime = 0;

    private Widget mainLayout;

    private ImageWidget m_ParkingStatus;
    private TextWidget m_ParkingText;

    private TextListboxWidget m_VirtualGarageList;
    private ItemPreviewWidget m_PrincipalItemVisual;

    private ButtonWidget m_ButtonStoreVehicle;
    private ButtonWidget m_ButtonGetVehicle;
    private ButtonWidget m_ButtonClose;

    private ref TStringArray m_VehiclesName;
    private ref TStringArray m_VehiclesListName;
    private EntityAI m_ItemVisual;

    private CarScript m_VehicleParking;
    private int m_VehicleSelected;

    vector m_ParkingPosition;
    vector m_ParkingDirection;
    vector m_ParkingOrientation;

    void Fabo_VirtualGarageMenu()
    {
        m_VehiclesListName = new array<string>;
        m_VehiclesName = new array<string>;
        m_VehicleParking = NULL;
    }

    void Show(bool show)
    {
        GetGame().GetUIManager().ShowUICursor(show);
        GetGame().GetMission().GetHud().Show(!show);

        if (!show)
        {
            GetGame().GetInput().ResetGameFocus();
            OnHide();
        }
        else
        {
            int focus = 1;
            GetGame().GetInput().ChangeGameFocus(focus);
            OnShow();
        }

        mainLayout.Show(show);
    }

    override void OnShow()
    {
        super.OnShow();

        float blur = 0.5;
        PPEffects.SetBlurMenu(blur);

        int focus = 1;
        GetGame().GetInput().ChangeGameFocus(focus);

        SetFocus( mainLayout );
    }

    override void OnHide()
    {
        super.OnHide();

        float blur = 0.0;
        PPEffects.SetBlurMenu(blur);

        GetGame().GetInput().ResetGameFocus();

        if (m_ItemVisual)
            GetGame().ObjectDelete(m_ItemVisual);

        Close();
    }

    override Widget Init()
    {
        mainLayout = GetGame().GetWorkspace().CreateWidgets( "VirtualGarage/gui/Fabo_VirtualGarageUI.layout" );

        m_VirtualGarageList = TextListboxWidget.Cast( mainLayout.FindAnyWidget( "GarageList" ) );

        m_ButtonStoreVehicle = ButtonWidget.Cast( mainLayout.FindAnyWidget( "ButtonStoreVehicle" ) );
        m_ButtonGetVehicle = ButtonWidget.Cast( mainLayout.FindAnyWidget( "ButtonGetVehicle" ) );
        m_ButtonClose = ButtonWidget.Cast( mainLayout.FindAnyWidget( "ButtonClose" ) );

        m_ParkingStatus  = ImageWidget.Cast(mainLayout.FindAnyWidget( "ParkingStatus" ));
        m_ParkingText  = TextWidget.Cast(mainLayout.FindAnyWidget( "ParkingText" ));

        mainLayout.Show(false);

        return mainLayout;
    }

    static CarScript getVehicleParking(vector position)
	{
	    int radius = 3;
		CarScript car;

		array<Object> items = new array<Object>;
		array<CargoBase> cargoBase = new array<CargoBase>;

		GetGame().GetObjectsAtPosition(position, radius, items, cargoBase);

		foreach(EntityAI item: items)
		{
		    if (Class.CastTo(car, item))
		        return car;
		}

		return NULL;
	}

    void GetListVehicles()
    {
        ClearList();

        m_VehicleParking = getVehicleParking(m_ParkingPosition);

        ChangeListWithParking(m_VehicleParking);

        ChangeParkingStatus(m_VehicleParking);

        ClearVehicleSelected();
    }

    void ClearList()
    {
        m_VehiclesListName.Clear();
        m_VirtualGarageList.ClearItems();
        m_VehicleSelected = -1;
    }

    void ChangeListWithParking(CarScript m_VehicleParking)
    {
        if (m_VehicleParking)
        {
            m_VirtualGarageList.AddItem(m_VehicleParking.GetType(), NULL, 0);
            m_VirtualGarageList.SetItemColor(0, 0, ARGB(255, 191, 48, 48));

            m_VehiclesListName.Insert(m_VehicleParking.GetType());
        }

        for (int i = 0; i < m_VehiclesName.Count(); i++)
        {
            int pos = i;
            if (m_VehicleParking)
                pos++;

            m_VirtualGarageList.AddItem(m_VehiclesName[i], NULL, 0);
            m_VirtualGarageList.SetItemColor(pos, 0, ARGB(255, 50, 50, 50));

            m_VehiclesListName.Insert(m_VehiclesName[i]);
        }
    }

    void ChangeParkingStatus(CarScript m_VehicleParking)
    {
        if (m_VehicleParking)
        {
            m_ParkingStatus.SetColor(ARGB(255,191,48,48));
            m_ParkingText.SetText("#fabo_parking_unavailability");
        }
        else
        {
            m_ParkingStatus.SetColor(ARGB(255,0,255,0));
            m_ParkingText.SetText("#fabo_parking_availability");
        }
    }

    void ClearVehicleSelected()
    {
        if (m_VehiclesListName.Count() <= 0)
            return;

        m_VehicleSelected = 0;

        ChangeVehicleVisual(m_VehiclesListName[m_VehicleSelected]);
    }

    void SetDataVehicle(array<string> vehicles, vector pos, vector ori)
    {
        m_VehiclesName.Clear();
        m_VehiclesName = vehicles;
        m_ParkingPosition = pos;
        m_ParkingOrientation = ori;
    }

    override bool OnClick (Widget w, int x, int y, int button)
    {
        switch(w)
        {
            case m_ButtonStoreVehicle: StoreVehicle();
            return true;
            break;

            case m_ButtonGetVehicle: DeployVehicle();
            return true;
            break;

            case m_VirtualGarageList: SelectVehicle();
            return true;
            break;

            case m_ButtonClose: OnHide();
            return true;
            break;
        }
        return false;
    }

    void SelectVehicle()
    {
        m_VehicleSelected = m_VirtualGarageList.GetSelectedRow();

        if (m_VehicleSelected == -1)
            return;

        ChangeVehicleVisual(m_VehiclesListName[m_VehicleSelected]);
    }

    void StoreVehicle()
    {
        if (CheckClickRateLimit())
            return;

        int uniquePlayerId = GetPlayerUniqueId(GetGame().GetUserManager().GetTitleInitiator().GetUid());
        PlayerIdentity identity = GetGame().GetPlayer().GetIdentity();

        GetRPCManager().SendRPC("VirtualGarage", "StoreVehicleRPC",  new Param2<int, CarScript>(uniquePlayerId, m_VehicleParking), true, identity);
    }

    void DeployVehicle()
    {
        if (CheckClickRateLimit())
            return;

        int uniquePlayerId = GetPlayerUniqueId(GetGame().GetUserManager().GetTitleInitiator().GetUid());
        PlayerIdentity identity = GetGame().GetPlayer().GetIdentity();

        if (m_VehicleParking)
            m_VehicleSelected--;

        GetRPCManager().SendRPC("VirtualGarage", "DeployVehicleRPC",  new Param4<vector, vector, int, int>(m_ParkingPosition, m_ParkingOrientation, uniquePlayerId, m_VehicleSelected), true, identity);
    }

    bool CheckClickRateLimit()
    {
        float currentTime = GetGame().GetTime();
        int seconds = 1 * 1000;

        if (currentTime - lastClickTime < seconds)
        {
            PlayerIdentity identity = GetGame().GetPlayer().GetIdentity();
            NotificationSystem.AddNotificationExtended(2, "#fabo_virtual_garage_title", "#fabo_wait_a_bit", "VirtualGarage/data/images/vglogo.paa");
            return true;
        }

        lastClickTime = currentTime;

        return false;
    }

    int GetPlayerUniqueId(string FullIdSteam)
	{
		string uniqueId = "";

		for (int i = 8; i < 17; i++)
			uniqueId += FullIdSteam.Get(i);

		return uniqueId.ToInt();
	}

    void ChangeVehicleVisual(string type)
    {
        GetVehicleVisual();

        SetVehicleVisual(type);
    }

    void GetVehicleVisual()
    {
        if ( m_PrincipalItemVisual )
           return;

        Widget VehicleVisual = mainLayout.FindAnyWidget("VehicleVisual");

        if (!VehicleVisual)
            return;

        float width;
        float height;
        VehicleVisual.GetSize(width, height);

        m_PrincipalItemVisual = ItemPreviewWidget.Cast( GetGame().GetWorkspace().CreateWidget(ItemPreviewWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, ARGB(255, 255, 255, 255), 10, VehicleVisual) );
    }

    void SetVehicleVisual(string type)
    {
        if (m_ItemVisual)
            GetGame().ObjectDelete( m_ItemVisual );

        vector vectorZero = Vector(0, 0, 0);

        m_ItemVisual = EntityAI.Cast(GetGame().CreateObject( type, vectorZero, true, false, true ));

        if (!m_ItemVisual)
            return;

        m_PrincipalItemVisual.SetItem(m_ItemVisual);
        m_PrincipalItemVisual.SetView(m_ItemVisual.GetViewIndex());

        m_PrincipalItemVisual.SetModelPosition(Vector(0, 0, 0.5));
        m_PrincipalItemVisual.SetModelOrientation(vectorZero);
    }
};
