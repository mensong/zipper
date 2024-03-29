﻿// TestZipper.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <zipper.h>
#include <unzipper.h>

int main()
{
	{
		zipper::Zipper z;
		if (!z.open("mensong.zip", "123456", zipper::Zipper::Overwrite))
			return 1;
		z.addFolder("tmp//", "tmp1\\/");
		z.add("..\\//zipper.sln", "sln1\\zipper.sln");
		z.close();
		z.reopen(zipper::Zipper::Append);
		z.addFolder("tmp//", "tmp2\\/");
		z.add("..\\//zipper.sln", "sln2\\zipper.sln");
		z.close();
	}

	//{
	//	zipper::Zipper z("mensong.zip", "123456", zipper::Zipper::Append);
	//	z.addFolder("tmp//", "tmp2\\/");
	//	z.add("..\\//zipper.sln", "sln2\\zipper.sln");
	//	z.close();
	//}

	zipper::Unzipper uz;
	if (!uz.open("mensong.zip", "123456"))
	{
		return 1;
	}
	uz.extract("mensong\\");
	uz.close();

	return 0;
}