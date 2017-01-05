#ifndef __CMDPARAMS_H__
#define __CMDPARAMS_H__

/*
 * \brief Аргументы командной строки
 */
struct CmdParams
{
	int ParticlesCount;
	int StepsCount; 
	char* ParamsFile;
	char* OutputFile;
	int NumberOfThreads;
};


#endif