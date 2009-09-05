/*
 * Configuration.cpp
 *
 *  Created on: Sep 4, 2009
 *      Author: vasile
 */

#include "Configuration.h"

Configuration* Configuration::instance = NULL;

Configuration* Configuration::Inst(){
  if(instance == NULL){
    instance = new Configuration();
  }
  return instance;
}


//private constructor
Configuration::Configuration(){
	Initialize();
}

Configuration::~Configuration(){
  if(instance != 0)delete instance;
}
void Configuration::Initialize(){
}
