/////////////////////////////////////////////////////////////////////////////
//	Project Name
//      VectorSortLin
/////////////////////////////////////////////////////////////////////////////
//	Description
//		Application: Sorting of Vectors in ulAMAoCP
//		Traditional "naked" thread implementation with CSP communication.
//		0003.VectorSortLin.png
//		CompSysTech '21 Paper: Solving Classical Problem in New Context as Constructive Model of Training
//		[csp namespace ver. 0.91RC]
//	Status: Under development.
/////////////////////////////////////////////////////////////////////////////
#pragma hdrstop
#pragma argsused

#ifdef _WIN32
#include <tchar.h>
#else
  typedef char _TCHAR;
  #define _tmain main
#endif

#include "UnitMain.h"
//---------------------------------------------------------------------------
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <thread>
#include <ctime>
#include <chrono>
//------------------------------------------------------------------------------
std::wofstream wof;         // Log file
//------------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	auto tp = std::chrono::system_clock::now();
	auto tt = std::chrono::system_clock::to_time_t(tp);

	std::wstringstream fn;
	// https://en.cppreference.com/w/cpp/io/manip/put_time
	fn << std::put_time(std::localtime(&tt), L"..\\..\\Logs\\Log.%Y%m%d.%H%M%S.txt");
	wof.open(fn.str());

	////////////////////////////////////////////////////////////////////////////
	//  COMMUNICAIONS MEDIUM
	////////////////////////////////////////////////////////////////////////////
	//
	//  Channels
	auto chanP0P1 = std::make_shared<CHAN>(L"P0P1");
	auto chanP1P2 = std::make_shared<CHAN>(L"P1P2");
	auto chanP2P3 = std::make_shared<CHAN>(L"P2P3");
	auto chanP3Q0 = std::make_shared<CHAN>(L"P3Q0");
	auto chanQ0Q1 = std::make_shared<CHAN>(L"Q0Q1");
	auto chanQ1Q2 = std::make_shared<CHAN>(L"Q1Q2");
	auto chanQ2Q3 = std::make_shared<CHAN>(L"Q2Q3");
	////////////////////////////////////////////////////////////////////////////

	std::cout << ">>> Application: Sorting of Vectors in ulAMAoCP" << std::endl;
	std::cout << "<<< Run >>>" << std::endl;

	////////////////////////////////////////////////////////////////////////////
	//  PROCESSES
	////////////////////////////////////////////////////////////////////////////
	std::vector<int> vSGMemorySrc = {1, 3, 9, 5};   // Unsorted Vector
	std::vector<int> vSGMemoryDst = {0, 0, 0, 0};   // Sorted Vector
	std::vector<std::thread> vThreads;
	//
	//	 S = {GatherGroup || ScatterGroup}
	//	 GatherGroup = {P0 || P1 || P2 || P3}
	//	 ScatterGroup = {Q0 || Q1 || Q2 || Q3}
	//
	auto tp1 = std::chrono::system_clock::now();    		// timepoint 1

	vThreads.push_back(std::thread(doP, 0, std::ref(vSGMemorySrc[0]), nullptr, chanP0P1));
	vThreads.push_back(std::thread(doP, 1, std::ref(vSGMemorySrc[1]), chanP0P1, chanP1P2));
	vThreads.push_back(std::thread(doP, 2, std::ref(vSGMemorySrc[2]), chanP1P2, chanP2P3));
	vThreads.push_back(std::thread(doP, 3, std::ref(vSGMemorySrc[3]), chanP2P3, chanP3Q0));
	//
	vThreads.push_back(std::thread(doQ, 0, std::ref(vSGMemoryDst[0]), chanP3Q0, chanQ0Q1));
	vThreads.push_back(std::thread(doQ, 1, std::ref(vSGMemoryDst[1]), chanQ0Q1, chanQ1Q2));
	vThreads.push_back(std::thread(doQ, 2, std::ref(vSGMemoryDst[2]), chanQ1Q2, chanQ2Q3));
	vThreads.push_back(std::thread(doQ, 3, std::ref(vSGMemoryDst[3]), chanQ2Q3, nullptr));
	//
	////////////////////////////////////////////////////////////////////////////
	//
	for(auto& t: vThreads)
	{
		if(t.joinable())
		{
			t.join();
		}
	}
	//
	////////////////////////////////////////////////////////////////////////////

	auto tp2 = std::chrono::system_clock::now();    		// timepoint 2
	auto delay = std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count();

	std::cout << ">>> End <<<" << '\n' << '\n';

	std::wstringstream wss;
	wss << "Run Time: " << delay << ", ms" << '\n' << '\n';

	wss << "Src:" << '\n';

	for(auto& i: vSGMemorySrc)
	{
		wss << i << '\n';
	}

	wss << '\n';
	wss << "Dst:" << '\n';

	for(auto& i: vSGMemoryDst)
	{
		wss << i << '\n';
	}

	std::wcout << wss.str() << '\n';
	wof << wss.str();

	if(wof.good())
	{
		std::wcout << L"The Log saved to <" << fn.str() << L">" << '\n';
	}
	wof.close();

	// "Press any key to continue..."
	system("pause");

	return 0;
}
//------------------------------------------------------------------------------
//  Main function of process P
//  P0 = P1 = P2 = P3 = P
void doP(int pid, int& x, CHAN_PTR in, CHAN_PTR out)
{
//	out->send(src);
}
//------------------------------------------------------------------------------
//  Main function of process Q
//  Q0 = Q1 = Q2 = Q3 = Q
void doQ(int pid, int& x, CHAN_PTR in, CHAN_PTR out)
{
//	int src;
//	in->recv(src);
//	dst = src + dst;
}
//------------------------------------------------------------------------------

