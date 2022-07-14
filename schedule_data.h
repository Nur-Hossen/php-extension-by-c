#ifndef SCHEDULE_DATA_H
#define	SCHEDULE_DATA_H

#include<math.h>
#include"boost/date_time/gregorian/gregorian.hpp"
#include "fstream"


class schedule_data {
    
public:
    int id;
    static int schedule_counter;
    
schedule_data(){
    id=++schedule_counter;
    std::ofstream fout;
    fout.open("/var/www/microfin360/trunk/application/logs/microfin360/scheduler.log",std::ios::app);
    fout<<"Schedule Data: "<<id<<" Created\n";
}

/*
schedule_data(boost::gregorian::date schedule_date, double installment_amount, double principle_installment_amount, double interrest_installment_amount){
    id=++schedule_counter;
    std::ofstream fout;
    fout.open("/var/www/microfin360/trunk/application/logs/microfin360/scheduler.log",std::ios::app);
    fout<<"Schedule Data: "<<id<<" Created\n";
    
	this->schedule_date=schedule_date;
	this->installment_amount=installment_amount;
	this->principle_installment_amount=principle_installment_amount;
	this->interrest_installment_amount=interrest_installment_amount;
	
}
*/
schedule_data(boost::gregorian::date schedule_date, double installment_amount, double principle_installment_amount, double interrest_installment_amount,double real_principle_installment_amount,double real_interrest_installment_amount){
    id=++schedule_counter;
    std::ofstream fout;
    fout.open("/var/www/microfin360/trunk/application/logs/microfin360/scheduler.log",std::ios::app);
    fout<<"Schedule Data: "<<id<<" Created\n";
    
	this->schedule_date=schedule_date;
	this->installment_amount=installment_amount;
        
        /*
	this->principle_installment_amount=principle_installment_amount;
	this->interrest_installment_amount=interrest_installment_amount;
        
        this->real_principle_installment_amount=real_principle_installment_amount;
        this->real_interrest_installment_amount=real_interrest_installment_amount;
        */
        
        this->principle_installment_amount=real_principle_installment_amount;
	this->interrest_installment_amount=real_interrest_installment_amount;
        
        this->real_principle_installment_amount=principle_installment_amount;
        this->real_interrest_installment_amount=interrest_installment_amount;
	
}


schedule_data(const schedule_data& orig){
    
        std::ofstream fout;
        fout.open("/var/www/microfin360/trunk/application/logs/microfin360/scheduler.log",std::ios::app);
        fout<<"Copy Schedule Data: "<<orig.id<<" Created\n";
        this->id=orig.id;
	this->schedule_date=orig.schedule_date;
	this->installment_amount=orig.installment_amount;
	this->principle_installment_amount=orig.principle_installment_amount;
	this->interrest_installment_amount=orig.interrest_installment_amount;
        
        this->real_principle_installment_amount=orig.real_principle_installment_amount;
        this->real_interrest_installment_amount=orig.real_interrest_installment_amount;
	
}
~schedule_data(){
    std::ofstream fout;
    fout.open("/var/www/microfin360/trunk/application/logs/microfin360/scheduler.log",std::ios::app);
    fout<<"Schedule Data: "<<id<<" Destroyed\n";
}


boost::gregorian::date schedule_date;
double installment_amount;
double principle_installment_amount;
double interrest_installment_amount;

double real_principle_installment_amount;
double real_interrest_installment_amount;

int samity_id;
};

#endif	/* SCHEDULE_DATA_H */
