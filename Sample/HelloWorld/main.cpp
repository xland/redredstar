#include <Windows.h>
#include <RRS/App.h>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow) 
{
	RRS::App::Init(hInstance);
	return RRS::App::Exec();
}