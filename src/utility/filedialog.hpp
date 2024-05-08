#pragma once

#include <nfd.hpp>
#include <string>

namespace Utils {
	
	class FileDialog
	{
	public:
		enum Error { None, FileDidntOpened };
		
		struct Path {
		    std::string out;
			Error err;
		};
		
		inline static FileDialog& Get() { static FileDialog dlg; return dlg; }

		Path Open(nfdfilteritem_t filterItem);
		Path Save(nfdfilteritem_t filterItem);

	private:
		FileDialog();
		
	    NFD::Guard ms_guard;
		
	};
	
}
