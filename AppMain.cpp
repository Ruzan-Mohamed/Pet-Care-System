#include "LoginForm.h"
#include "MainForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<System::String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    while (true)
    {
        // 1. Show Login Screen
        PawCare::LoginForm^ login = gcnew PawCare::LoginForm();
        if (login->ShowDialog() != DialogResult::OK || !login->IsLoginSuccessful)
        {
            break; // User cancelled or closed login dialog
        }

        // 2. Open Main Dashboard upon successful authentication
        PawCare::MainForm^ mainForm = gcnew PawCare::MainForm(login->CurrentUser, login->CurrentRole);
        Application::Run(mainForm);

        // If user logged out, loop back to show LoginForm again; otherwise exit
        if (!mainForm->IsLoggedOut)
        {
            break;
        }
    }

    return 0;
}
