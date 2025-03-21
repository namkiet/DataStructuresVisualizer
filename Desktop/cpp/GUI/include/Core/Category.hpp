#pragma once

namespace Category
{
	enum Type
	{
		None				= 0,
		Scene				= 1 << 0,
		RootNode 			= 1 << 1,
		Node	 	       	= 1 << 2,
		TreeNode	 	    = 1 << 3
	};
}
