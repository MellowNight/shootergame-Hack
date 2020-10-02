class HotkeyListener
{
public:
    virtual void OnHotkeyPressed() = 0;
};

struct HotkeyButton
{
    HotkeyButton(UINT defaultKey, HotkeyListener* callback);

    UINT key;
    bool waitingForKey = false;
    HotkeyListener* cb;
};