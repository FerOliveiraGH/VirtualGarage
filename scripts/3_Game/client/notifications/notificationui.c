modded class NotificationUI
{
    override void UpdateOffset()
	{
		UIScriptedMenu menu = UIScriptedMenu.Cast(GetGame().GetUIManager().GetMenu());
		if (menu)
		{
			Widget fabo_LayoutRoot = menu.GetLayoutRoot();
			Widget expNotification;

			if (fabo_LayoutRoot)
                expNotification = fabo_LayoutRoot.FindAnyWidget("notification_root");

            if (expNotification && expNotification.IsVisible())
            {
                if (!m_OffsetEnabled)
                {
                    m_Root.GetPos(m_BackupPosX, m_BackupPosY);

                    float x, y, w, h;
                    m_Root.GetScreenPos(x, y);
                    expNotification.GetScreenSize(w, h);

                    m_Root.SetScreenPos(x, h);
                    m_OffsetEnabled = true;
                }
            }
            else if (m_OffsetEnabled)
            {
                m_Root.SetPos(m_BackupPosX, m_BackupPosY);
                m_OffsetEnabled = false;
            }
		}
	}
}