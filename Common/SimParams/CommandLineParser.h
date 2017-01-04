#ifndef __COMMANDLINEPARSER_H__
#define __COMMANDLINEPARSER_H__

#include "SimParams.h"
#include "CmdParams.h"

#include <iostream>
#include <cstring>
#include <fstream>

/*!
 * \brief Парсит аргументы командной строки
 *
 * Аргументы вида:
 *    binary  -particles <num of particles> -steps <num of steps> -params <simulation params file>  -out <output file> [-nt <number of threads>]
 *
 * Вид файла параметров:
 *   <Dt>
 *   <Point mass>
 *   <G>
 *   <Min dist>
 *   <Radius>
 *
 * \param[in] argc Количество аргументов
 * \param[in] argv Аргументы
 * \param[out] params Параметры симуляции
 * \param[out] numberOfThreads Число потоков (-1 - не задано)
 */
bool ParseCommandLine(int argc, char** argv, CmdParams& params);

/*!
 * \brief  Читает параметры из файла конфигурации
 */
bool LoadParamsFile(const char* filename, SimParams& params);

#endif
