#include<iostream>
#include<getopt.h>
#include"EncFile.hpp"
#include<Application.h>
#include<Window.h>


static struct option long_options[] =
{
    {"key", required_argument, NULL, 'k'},
    {"iv", required_argument, NULL, 'v'},
    {"file", required_argument, NULL, 'f'},
    {"shred", 0, NULL, 's'},
    //{"init_new", required_argument, NULL, 'n'},
    {"help", 0, NULL, 'h'},
    {"gui", 0, NULL, 'g'},
    {NULL, 0, NULL, 0}
};
void help(void)
{
    std::cout << "Usage program: --key/-k your key password" << std::endl;
    std::cout << "Usage program: --iv/-v your iv" << std::endl;
    std::cout << "Usage program: --file/-f your file" << std::endl;
    std::cout << "Usage program: --init_new/-n (not implemented for a now)" << std::endl;
    std::cout << R"(/a.out --key "Hello World" --iv "123" -f SuperText.txt)" << std::endl;;
    std::cout << R"(./a.out --key "Hello World" --iv "123" -f SuperText.txt.enc)" << std::endl;;
}
class MEncryptorApp : public BApplication 
{
	protected:
		BWindow * m_window;
		BRect * m_mainRect;
	public:
	MEncryptorApp(void) : BApplication("application/x-vnd.MENcryptorAPP") {
		m_mainRect = new BRect(0,0, 256, 256);
		m_window = new BWindow(*m_mainRect, 
				"MEncryptorAPP",
				B_DOCUMENT_WINDOW_LOOK,
				B_NORMAL_WINDOW_FEEL,
				0
				);
		m_window->Show();
	}
};
int launch_gui(void)
{
	auto app = new MEncryptorApp{};
	app -> Run();
	delete app;
	return 0;
}

int main(int argc, char ** argv, char ** env)
{
    char ch;
    std::string key, iv, filepath;
    //bool init_new;
    bool shred_file = false;
    while ((ch = getopt_long(argc, argv, "gk:v:f:n:hs", long_options, NULL)) != -1)
    {
        switch (ch)
        {
            case 's':
                shred_file = true;
                break;
	    case 'g':
		return launch_gui();
		break;
            case 'k':
                key = optarg;
                break;
            case 'v':
                iv = optarg;
                break;
            case 'f':
                filepath = optarg;
                break;
            default:
            case 'h':
                help();
                return 0;
        }
    }
    if (shred_file)
    {
        if(filepath.empty()) {
            std::cerr << "Give a filepath for shred" << std::endl;
            return 1;
        }
        auto size = std::filesystem::file_size(filepath);
        for(unsigned char i=33;i--;)
        {
            std::ofstream f{filepath};
            for(auto s = size;s--;)
            {
                f << i;
            }
        }
        return 0;
    }
    if (filepath.empty() || key.empty() || iv.empty())
    {
        help();
        return 0;
    }
    //srand(time(NULL));
    CounterChaCha20 = 33;//rand();
    // ^^^ TODO:
    if (filepath.ends_with(".enc"))
    {
        std::cout << "Decrypt file: " + filepath << std::endl;
        auto f = EncFile{filepath, key, iv, false};
        //f << "Test text";
        //std::cout << f.encdecrypt() << std::endl;
        std::cout << "Check your file in .dec" << std::endl;
    } else
    {
        std::cout << "Encrypt file: " + filepath << std::endl;
        auto f = EncFile{filepath, key, iv, true};
        //f << "Test text";
        //std::cout << f.encdecrypt() << std::endl;
        std::cout << "Check your file in .enc" << std::endl;
    }
    std::cout << "After usage you can want to shred the file" << std::endl;
}
