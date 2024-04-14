#include <filedialog.hpp>

#include <nfd.hpp>

namespace Utils {

	FileDialog::FileDialog()
		: ms_guard()
	{ }
	
	FileDialog::Path FileDialog::Open(nfdfilteritem_t filterItem)
	{
	    nfdu8char_t* path; // nfdu8char_t*& outPath
		auto res = NFD::OpenDialog(path, &filterItem, 1);

		Error err;
		if (res == NFD_OKAY)
			err = FileDialog::None;
		else if (res == NFD_CANCEL)
			err = FileDialog::FileDidntOpened;
			
		return { path, err };
	}

	FileDialog::Path FileDialog::Save(nfdfilteritem_t filterItem)
	{
	    nfdu8char_t* path; // nfdu8char_t*& outPath
		auto res = NFD::SaveDialog(path, &filterItem, 1);

		Error err;
		if (res == NFD_OKAY)
			err = FileDialog::None;
		else if (res == NFD_CANCEL)
			err = FileDialog::FileDidntOpened;
			
		return { path, err };
	}
	
}
