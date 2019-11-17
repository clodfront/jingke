// Project:	jingke
// File:    main.cpp
// Created: 11/2019
// Author:  Goof
//
// Copyright 2019-2020 Goof
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include <iostream>
#include "logicserver.h"


using namespace std;

int main(int argc, char** argv)
{
	if(argc < 2) 
	{
		cout << "argc < 2" << endl;
		return -1;
	}

	CLogicServerLike svr;
	int ret = svr.Init(argv[1]);
	if(ret != 0)
	{
		cout << "svr.Init fail. ret:" << ret << endl;
		return -2;
	}
	svr.Run();

	return 0;
}
