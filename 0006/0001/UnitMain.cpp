/////////////////////////////////////////////////////////////////////////////
//	Project Name
//      VectorCopy
/////////////////////////////////////////////////////////////////////////////
//	Description
//		Application: Copy of Vectors in AMAoCP
//      Traditional "naked" thread implementation with CSP communication.
//		0001.VectorCopy.png
//		[csp namespac ver. 0.91RC]
//	Status: Completed
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
	auto chanP0Q0 = std::make_shared<CHAN>(L"P0Q0");
	auto chanP1Q1 = std::make_shared<CHAN>(L"P1Q1");
	auto chanP2Q2 = std::make_shared<CHAN>(L"P2Q2");
	auto chanP3Q3 = std::make_shared<CHAN>(L"P3Q3");
	////////////////////////////////////////////////////////////////////////////

	std::cout << ">>> Application: Copy of Vectors in AMAoCP" << std::endl;
	std::cout << "<<< Run >>>" << std::endl;

	////////////////////////////////////////////////////////////////////////////
	//  PROCESSES
	////////////////////////////////////////////////////////////////////////////
	std::vector<int> vSGMemorySrc = {100, 101, 102, 103};
	std::vector<int> vSGMemoryDst = {0, 0, 0, 0};
	std::vector<std::thread> vThreads;
	//
	//	 S = {Pair0 || Pair1 || Pair2 || Pair3}
	//	 Pair0 = {P0 || Q0}
	//	 Pair1 = {P1 || Q1}
	//	 Pair2 = {P2 || Q2}
	//	 Pair3 = {P3 || Q3}
	//
	auto tp1 = std::chrono::system_clock::now();    		// timepoint 1

	vThreads.push_back(std::thread(doP, 0, std::ref(vSGMemorySrc[0]), chanP0Q0));
	vThreads.push_back(std::thread(doP, 1, std::ref(vSGMemorySrc[1]), chanP1Q1));
	vThreads.push_back(std::thread(doP, 2, std::ref(vSGMemorySrc[2]), chanP2Q2));
	vThreads.push_back(std::thread(doP, 3, std::ref(vSGMemorySrc[3]), chanP3Q3));
	//
	vThreads.push_back(std::thread(doQ, 0, std::ref(vSGMemoryDst[0]), chanP0Q0));
	vThreads.push_back(std::thread(doQ, 1, std::ref(vSGMemoryDst[1]), chanP1Q1));
	vThreads.push_back(std::thread(doQ, 2, std::ref(vSGMemoryDst[2]), chanP2Q2));
	vThreads.push_back(std::thread(doQ, 3, std::ref(vSGMemoryDst[3]), chanP3Q3));
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

	for(auto& i: vSGMemoryDst)
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
//  P1 = P2 = P3 = P4 = P
void doP(int pid, int& src, CHAN_PTR out)
{
	// P = {Q ! src}
	out->send(src);
}
//------------------------------------------------------------------------------
//  Main function of process Q
//  Q1 = Q2 = Q3 = Q4 = Q
void doQ(int pid, int& dst, CHAN_PTR in)
{
	// Q = {P ? dst}
	in->recv(dst);
}
//------------------------------------------------------------------------------

