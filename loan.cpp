/*
 * File:   loan.cpp
 * Author: Murad
 *
 * Created on February 6, 2011, 4:57 PM
 */


#include "loan.h"
#include <math.h>
#include <iostream>
#include <fstream>

double my_round(double d){
    /*
    double e=round(d*1000.0);
    d=e/10;
    e=round(d);
    d=e/100.0;
    char buffer[100];
    sprintf(buffer,"%.2f",d);
    e=strtod(buffer,NULL);
    return e;
    */
    //return d;
    //std::cerr<<d<<"\n";
    //d=lround(lround(d*1000)/10)/100;
    //d=(lround(d*100)/100);
    d=(round(d*100)/100);
    //std::cerr<<d<<"\n";
    
    char buffer[100];
    sprintf(buffer,"%.2f",d);
    d=strtod(buffer,NULL);
    ////exit(1);
    return d;
}
double my_round4(double d){
    /*
    double e=round(d*1000.0);
    d=e/10;
    e=round(d);
    d=e/100.0;
    char buffer[100];
    sprintf(buffer,"%.2f",d);
    e=strtod(buffer,NULL);
    return e;
    */
    //return d;
    //std::cerr<<d<<"\n";
    //d=lround(lround(d*1000)/10)/100;
    //d=(lround(d*100)/100);
    d=(round(d*10000)/10000);
    //std::cerr<<d<<"\n";
    
    char buffer[100];
    sprintf(buffer,"%.4f",d);
    d=strtod(buffer,NULL);
    ////exit(1);
    return d;
}
double my_round3(double d){
    /*
    double e=round(d*1000.0);
    d=e/10;
    e=round(d);
    d=e/100.0;
    char buffer[100];
    sprintf(buffer,"%.2f",d);
    e=strtod(buffer,NULL);
    return e;
    */
    //return d;
    //std::cerr<<d<<"\n";
    //d=lround(lround(d*1000)/10)/100;
    //d=(lround(d*100)/100);
    d=(round(d*1000)/1000);
    //std::cerr<<d<<"\n";
    
    char buffer[100];
    sprintf(buffer,"%.3f",d);
    d=strtod(buffer,NULL);
    ////exit(1);
    return d;
}

double my_trim(double d){
    //d=my_round(d);
    char buffer[100];
    sprintf(buffer,"%.2f",d);
    d=strtod(buffer,NULL);
    
    return d;
}
loan::loan() {
}

loan::loan(
        __gnu_cxx::hash_map<std::string, double> one_time_weekly_loans_transactions__info,
        __gnu_cxx::hash_map<std::string, int> loan_transfer_samitychange__info,
        __gnu_cxx::hash_map<std::string, std::string> loan_transfer_daychange__info,
        int id,
        int branch_id = 0,
        int samity_id = 0,
        int product_id = 0,
        int purpose_id=0,
        int member_id = 0,
        double loan_amount = 0.0,
        double interest_rate = 0.0,
        int number_of_installment = 0,
        std::string repayment_frequency = "",
        std::string grace_period = "",
        double loan_period_in_month = 0,
        boost::gregorian::date first_repayment_date = boost::gregorian::day_clock::local_day(),
        boost::gregorian::date last_repayment_date = boost::gregorian::day_clock::local_day(),
        boost::gregorian::date disburse_date = boost::gregorian::day_clock::local_day(),
        boost::gregorian::date loan_fully_paid_date = boost::gregorian::day_clock::local_day(),
        int current_status = 0,
       int is_loan_fully_paid=0,
       int is_authorized=0,
       std::string interest_calculation_method="",
       std::string mode_of_interest="",
       boost::gregorian::date loan_closing_date=boost::gregorian::day_clock::local_day(),
       double extra_amount=0,
       int original_loan_id=0,
       int is_transferred_in=0,
       std::string transfer_in_date="",
       int is_transferred_out=0,
       std::string transfer_out_date="" ,
       double penalty=0,
       double rebate=0,
       boost::gregorian::date rebate_date=boost::gregorian::day_clock::local_day(),
       double waiver=0,
       std::string loan_type="R",
       boost::gregorian::date *system_date=NULL,
       std::string interest_payment_frequency="",
       std::string repayment_mode_for_monthly_loan="",
       std::string repayment_day_or_week="",
       double interest_index=0,
       int enforce_number_of_installment=0,
       int enforced_install_number=0,
       std::string decline_period="",
       int is_show_rec_after_loan_period=0 ,
       double installment_amountdb=0,
       double loan_period_in_day = 0,
       int is_loan_period_in_day_fixed=0,
       int skip_last_week_calculation_monthly=0,
       std::string service_charge_taken_time="",      
       int cycle=1

        ) {
                   this->enforce_number_of_installment=enforce_number_of_installment;
                   this->is_show_rec_after_loan_period=is_show_rec_after_loan_period;
                   this->enforced_install_number=enforced_install_number;
                   this->system_date=system_date;
                   //std::cerr<<"System Date "<<this->system_date<<"\n";
                   
                   this->interest_payment_frequency=interest_payment_frequency;
                   this->repayment_mode_for_monthly_loan=repayment_mode_for_monthly_loan;
                   this->service_charge_taken_time=service_charge_taken_time;
                   this->repayment_day_or_week=repayment_day_or_week;
                   
                   if(this->repayment_mode_for_monthly_loan=="DD"){
                       
                   }
                   
                   this->loan_type=loan_type;
                   std::ofstream fout;
                   fout.open("/var/www/microfin360/trunk/application/logs/microfin360/scheduler.log",std::ios::app);
                   fout<<"Loan ID: "<<id<<" Created\n";
                   
		   this->id=id;
		   this->branch_id=branch_id;
		   this->samity_id=samity_id;
		   this->product_id=product_id;
                   this->purpose_id=purpose_id;
		   this->member_id=member_id;
		   this->loan_amount=loan_amount;
                   this->installment_amountdb=installment_amountdb;
		   this->interest_rate=interest_rate;
		   this->number_of_installment=number_of_installment;
		   this->repayment_frequency=repayment_frequency;
		   this->grace_period=grace_period;
		   this->loan_period_in_month=loan_period_in_month;
                   this->loan_period_in_day=loan_period_in_day;
                   this->is_loan_period_in_day_fixed=is_loan_period_in_day_fixed;
		   this->first_repayment_date=first_repayment_date;
		   this->last_repayment_date=last_repayment_date;
		   this->disburse_date=disburse_date;
		   this->loan_fully_paid_date=loan_fully_paid_date;
		   this->current_status=current_status;
		   this->is_loan_fully_paid=is_loan_fully_paid;
		   this->is_authorized=is_authorized;
                   this->interest_calculation_method=interest_calculation_method;
                   this->decline_period=decline_period;        
                   this->mode_of_interest=mode_of_interest;
                   this->loan_closing_date=loan_closing_date;

                   this->is_transferred=0;
                   this->is_transferred_in=0;
                   this->is_transferred_out=0;

                   this->is_transferred_in=is_transferred_in;
                   this->is_transferred_in_flag=is_transferred_in;
                   this->is_transferred_out=is_transferred_out;
                   this->is_transferred_out_flag=is_transferred_out;
                   this->cycle=cycle;
                   if(is_transferred_in){
                       this->is_transferred=is_transferred_in;
                       this->transfer_in_date=boost::gregorian::from_simple_string(transfer_in_date);
                   }
                   if(is_transferred_out){
                       this->is_transferred=is_transferred_out;
                       this->transfer_out_date=boost::gregorian::from_simple_string(transfer_out_date);
                   }

                   this->original_loan_id=original_loan_id;
                   if(original_loan_id==0){
                       original_loan_id=id;
                   }
                   this->skip_last_week_calculation_monthly=skip_last_week_calculation_monthly;
                  std::string last_insert="";
                  for(__gnu_cxx::hash_map<std::string, std::string>::iterator it=loan_transfer_daychange__info.begin();it!=loan_transfer_daychange__info.end();it++){
                        this->loan_transfer_daychange__info[boost::gregorian::from_simple_string(it->first)]=it->second;

                  }
                  one_time_weekly_priciple_paid_amount=0;
                  for(__gnu_cxx::hash_map<std::string, double>::iterator it1=one_time_weekly_loans_transactions__info.begin();it1!=one_time_weekly_loans_transactions__info.end();it1++){
                        this->one_time_weekly_loans_transactions__info[boost::gregorian::from_simple_string(it1->first)]=it1->second;

                  }
                  for(__gnu_cxx::hash_map<std::string, int>::iterator it=loan_transfer_samitychange__info.begin();it!=loan_transfer_samitychange__info.end();it++){
                        this->loan_transfer_samitychange__info[boost::gregorian::from_simple_string(it->first)]=it->second;

                  }

                   //std::cerr<<"In Loan Creation\n";
                   for(__gnu_cxx::hash_map<boost::gregorian::date, std::string, boost::hash<boost::gregorian::date> >::iterator it = this->loan_transfer_daychange__info.begin();it != this->loan_transfer_daychange__info.end();it++){
                       //std::cerr<<boost::gregorian::to_iso_extended_string(it->first)<<" "<<it->second<<std::endl;
                   }
                   ////exit(1);

                   //this->loan_transfer_daychange__info=loan_transfer_daychange__info;
                   week_days["Sun"]=0;
                   week_days["Mon"]=1;
                   week_days["Tue"]=2;
                   week_days["Wed"]=3;
                   week_days["Thu"]=4;
                   week_days["Fri"]=5;
                   week_days["Sat"]=6;

                   this->penalty=penalty;
                   this->waiver=waiver;
                   this->rebate=rebate;
                   
                   this->rebate_date=rebate_date;
                   
                   //std::cerr<<"rebate "<<this->rebate<<"rebate_date "<<this->rebate_date<<" system_date"<<this->system_date<<std::endl;
                   ////exit(1);

                   loan_amnt=loan_amount;
                   if(this->loan_period_in_month==0){
                       this->loan_period_in_month=1;

                   }
                   if(this->loan_period_in_day==0){
                       this->loan_period_in_day=1;

                   }
                   
                   if(this->number_of_installment==0){
                       this->number_of_installment=1;

                   }
                   no_of_payment_per_year = (this->number_of_installment*12)/this->loan_period_in_month;
                   interest_per_installment=interest_rate/(no_of_payment_per_year*100);


                   this->extra_amount=extra_amount;
                   total_installment_amount=0;
                   total_interrest_amount=0;
                   total_principle_amount=0;
                   real_total_interrest_amount=0;
                   real_total_principle_amount=0;

                   this->interest_index=interest_index;
                   
                   actual_number_of_installments=this->number_of_installment;

                   if(this->interest_calculation_method.compare("F")==0){
                       
                       if(this->mode_of_interest.compare("H")==0){
                           
                           total_amnt= (loan_amount+interest_per_installment*number_of_installment*loan_amount);
                           
                           /**/
                           /*
                           char buffer[100];
                           sprintf(buffer,"%.2f",total_amnt);
                           total_amnt=strtod(buffer,NULL);
                           */
                           //total_amnt=my_trim(total_amnt);
                           total_amnt=round(total_amnt);
                           //std::cerr<<"total Amt "<< total_amnt << std::endl;
                           installment_amnt=total_amnt/number_of_installment+this->extra_amount;
                           //pri_installment_amnt=loan_amount/number_of_installment;
                           //ins_installment_amnt=installment_amnt-pri_installment_amnt;                           
                           //actual_number_of_installments=ceil(total_amnt/installment_amnt);
                           actual_number_of_installments=this->number_of_installment;
                           if(this->extra_amount>0){                           
                                pri_installment_amnt=my_round(installment_amnt/((this->loan_period_in_month/12)*interest_rate*0.01+1));
                           }
                           else{
                                pri_installment_amnt=loan_amount/actual_number_of_installments;
                                //std::cerr<<installment_amnt<<" "<<pri_installment_amnt<<" \n";

                                if(total_amnt-installment_amnt*(actual_number_of_installments-1)<pri_installment_amnt){
                                    pri_installment_amnt=floor(loan_amount/(actual_number_of_installments-1));
                                }
                           }
                           //if(loan_amount-pri_installment_amnt*(actual_number_of_installments-1)<0){    
                           //}
                           //std::cerr<<installment_amnt<<" "<<pri_installment_amnt<<" "<<ins_installment_amnt<<"\n";
                           ins_installment_amnt=installment_amnt-pri_installment_amnt; 

                           //sprintf(buffer,"%.2f",installment_amnt);
                           //installment_amnt=strtod(buffer,NULL);
                           //installment_amnt=my_trim(installment_amnt);
                           installment_amnt=my_round(installment_amnt);

                           //sprintf(buffer,"%.2f",pri_installment_amnt);
                           //pri_installment_amnt=strtod(buffer,NULL);
                           //pri_installment_amnt=my_trim(pri_installment_amnt);
                           pri_installment_amnt=my_round(pri_installment_amnt);

                           //sprintf(buffer,"%.2f",ins_installment_amnt);
                           //ins_installment_amnt=strtod(buffer,NULL);
                           ins_installment_amnt=my_round(ins_installment_amnt);
                           
                           {
                                double last_pri_installment_amnt=loan_amount-pri_installment_amnt*(actual_number_of_installments-1);
                                double last_ins_installment_amnt=(total_amnt-loan_amount)-ins_installment_amnt*(actual_number_of_installments-1);
                                double last_installment_amnt=last_pri_installment_amnt+last_ins_installment_amnt;
                                last_installment_amnt=ceil(my_trim(last_installment_amnt));
                                last_ins_installment_amnt=last_installment_amnt-last_pri_installment_amnt;
                                double n_total_repay=my_trim(last_installment_amnt+installment_amnt*(actual_number_of_installments-1));
                                double n_total_int=my_trim(last_ins_installment_amnt+ins_installment_amnt*(actual_number_of_installments-1));
                                if(n_total_repay!=loan_amount+n_total_int){
                                    ins_installment_amnt=ins_installment_amnt+0.01;
                                    pri_installment_amnt=installment_amnt-ins_installment_amnt;
                                }
                                    
                           }
                            
                       }
                       else if(this->mode_of_interest.compare("T")==0){
                           interest_per_installment=interest_rate/(no_of_payment_per_year*1000);
                           total_amnt= (loan_amount+interest_per_installment*number_of_installment*loan_amount);

                           //char buffer[100];
                           //sprintf(buffer,"%.2f",total_amnt);
                           //total_amnt=strtod(buffer,NULL);
                           //total_amnt=my_trim(total_amnt);
                           total_amnt=round(total_amnt);

                           installment_amnt=total_amnt/number_of_installment+this->extra_amount;
                           //pri_installment_amnt=loan_amount/number_of_installment;
                           //ins_installment_amnt=installment_amnt-pri_installment_amnt;
                           actual_number_of_installments=ceil(total_amnt/installment_amnt);
                           pri_installment_amnt=loan_amount/actual_number_of_installments;
                           if(total_amnt-installment_amnt*(actual_number_of_installments-1)<pri_installment_amnt){
                               pri_installment_amnt=floor(loan_amount/(actual_number_of_installments-1));
                           }
                           ins_installment_amnt=installment_amnt-pri_installment_amnt;

                           //sprintf(buffer,"%.2f",installment_amnt);
                           //installment_amnt=strtod(buffer,NULL);
                           //installment_amnt=my_trim(installment_amnt);
                           installment_amnt=my_round(installment_amnt);

                           //sprintf(buffer,"%.2f",pri_installment_amnt);
                           //pri_installment_amnt=strtod(buffer,NULL);
                           //pri_installment_amnt=my_trim(pri_installment_amnt);
                           pri_installment_amnt=my_round(pri_installment_amnt);

                           //sprintf(buffer,"%.2f",ins_installment_amnt);
                           //ins_installment_amnt=strtod(buffer,NULL);
                           ins_installment_amnt=my_round(ins_installment_amnt);
                           
                           {
                                double last_pri_installment_amnt=loan_amount-pri_installment_amnt*(actual_number_of_installments-1);
                                double last_ins_installment_amnt=(total_amnt-loan_amount)-ins_installment_amnt*(actual_number_of_installments-1);
                                double last_installment_amnt=last_pri_installment_amnt+last_ins_installment_amnt;
                                //last_installment_amnt=ceil(my_trim(last_installment_amnt));
                                last_installment_amnt=ceil(my_round(last_installment_amnt));
                                last_ins_installment_amnt=last_installment_amnt-last_pri_installment_amnt;
                                //double n_total_repay=my_trim(last_installment_amnt+installment_amnt*(actual_number_of_installments-1));
                                double n_total_repay=my_round(last_installment_amnt+installment_amnt*(actual_number_of_installments-1));
                                //double n_total_int=my_trim(last_ins_installment_amnt+ins_installment_amnt*(actual_number_of_installments-1));
                                double n_total_int=my_round(last_ins_installment_amnt+ins_installment_amnt*(actual_number_of_installments-1));
                                if(n_total_repay!=loan_amount+n_total_int){
                                    ins_installment_amnt=ins_installment_amnt+0.01;
                                     pri_installment_amnt=installment_amnt-ins_installment_amnt;
                                }
                                    
                           }
                       }
                       total_payable_amount=total_amnt;
                       this->interest_index=this->total_amnt/this->loan_amnt;
                           
                   }
                   else if(this->interest_calculation_method.compare("D")==0)
                   {
                       if(this->mode_of_interest.compare("T")==0){
                           interest_per_installment=interest_rate/(no_of_payment_per_year*1000);
                       }

                       installment_amnt=(loan_amount * interest_per_installment * pow((interest_per_installment + 1), number_of_installment)) / (pow((interest_per_installment + 1), number_of_installment) - 1);
                       //total_amnt= installment_amnt*number_of_installment;
                       //if(extra_amount>0)
                       installment_amnt=my_round(installment_amnt)+extra_amount;
                       actual_number_of_installments=ceil(my_round((log10(installment_amnt/(installment_amnt-loan_amount * interest_per_installment))/log10(interest_per_installment + 1))));

//                     ********  update for Bug #14532: Monthly loan last installment amount & schedule problem for Declining method ***********//
                        if(actual_number_of_installments > this->number_of_installment){
                            actual_number_of_installments = this->number_of_installment;
                        }

                       total_payable_amount=(loan_amount * pow((interest_per_installment + 1), actual_number_of_installments))-(installment_amnt / interest_per_installment * pow((interest_per_installment + 1), actual_number_of_installments))+(installment_amnt / interest_per_installment * (interest_per_installment + 1))+installment_amnt*(actual_number_of_installments-1);
                       //std::cerr<<" total_payable_amount "<<total_payable_amount<<std::endl;
                       //total_amnt=my_round(total_payable_amount);
                       //ins_installment_amnt=loan_amnt*interest_per_installment;
                       //pri_installment_amnt=installment_amnt-ins_installment_amnt;

                       return;
                   }
                   else{                                                        
                       if(this->decline_period.compare("DB")==0 && this->number_of_installment==1 && this->service_charge_taken_time.compare("SCTBI")!=0){
                            this->actual_number_of_installments=this->number_of_installment;
                            
                            boost::gregorian::date_duration d_D=(this->first_repayment_date-this->disburse_date);                            
                            int no_of_D=d_D.days();
                            
                            if(this->loan_period_in_day > 1){
                                no_of_D=this->loan_period_in_day;
                            }
                            
                            if(this->mode_of_interest.compare("T")==0){
                                total_payable_amount=round((loan_amount*this->interest_index*no_of_D)/1000);  
                            }
                            else{
                                total_payable_amount=round((loan_amount*this->interest_index*no_of_D)/100);   
                            }
                            total_payable_amount=total_payable_amount+loan_amount;
                            installment_amnt=total_payable_amount;
                            interest_per_installment=total_payable_amount;
                            if(enforce_number_of_installment==0)                        
                            actual_number_of_installments=ceil(total_payable_amount/installment_amnt);
                            total_amnt=total_payable_amount;
                            installment_amount=total_payable_amount;  
                            //std::cerr<<" total_payable_amount "<<total_payable_amount<<"##"<<this->actual_number_of_installments<<std::endl;
                       }                       
                       else if(this->decline_period.compare("DB")==0 && this->number_of_installment==1 && this->service_charge_taken_time.compare("SCTBI")==0)
                       {                        
                           //std::cerr<<" repayment_mode_for_monthly_loan "<< this->repayment_mode_for_monthly_loan <<std::endl; 
                           
                           boost::gregorian::date_duration d_D=(this->first_repayment_date-this->disburse_date);
                           int no_of_D=d_D.days();
                           //std::cerr<<" no_of_D "<< no_of_D <<std::endl;
                           
                           if(this->loan_period_in_day > 1){
                                no_of_D=this->loan_period_in_day;
                            }
                           
                           if(this->mode_of_interest.compare("T")==0){
                                interest_per_installment= this->installment_amount;
                           }

                            this->interest_index = (this->interest_index);
                            //total_payable_amount=round(loan_amount*((this->interest_index-1)*(this->loan_period_in_month/12)+1));
                            
                            //std::cerr<<" interest_index "<<interest_index<<std::endl;
                            
                            //total_payable_amount=round(loan_amount*this->interest_index);
                            
                            total_payable_amount=round(loan_amount+loan_amount*this->interest_index * no_of_D /100);
                            
                            //std::cerr<<" interest_per_installment "<<interest_per_installment<<std::endl;
                            
                            installment_amnt=(loan_amount * interest_per_installment * pow((interest_per_installment + 1), number_of_installment)) / (pow((interest_per_installment + 1), number_of_installment) - 1);
                            //total_amnt= installment_amnt*number_of_installment;
                            //total_payable_amount-
                            //if(extra_amount>0)
                            installment_amnt=my_round(installment_amnt)+extra_amount;
                            installment_amnt=this->installment_amountdb;
                            //actual_number_of_installments=ceil(my_round((log10(installment_amnt/(installment_amnt-loan_amount * interest_per_installment))/log10(interest_per_installment + 1))));
                            if(enforce_number_of_installment==0)                            
                                 //actual_number_of_installments=ceil((total_payable_amount - loan_amount)/this->installment_amnt);
                           //actual_number_of_installments=23;
                           //std::cerr<<" number_of_installment "<<this->number_of_installment<<std::endl;
                           //std::cerr<<" installment_amount "<<installment_amnt<<std::endl;        
                                //actual_number_of_installments=ceil(my_round((log10(installment_amnt/(installment_amnt-loan_amount * interest_per_installment))/log10(interest_per_installment + 1))));
                           actual_number_of_installments =  ceil((total_payable_amount - loan_amount )/installment_amnt);  
                           //std::cerr<<" actual_number_of_installments "<<actual_number_of_installments<<std::endl;        
                           //installment_amnt=total_payable_amount/(this->number_of_installment);
                            //installment_amnt=round(installment_amnt+this->extra_amount);
                            //actual_number_of_installments=this->number_of_installment;

                            total_amnt=total_payable_amount;
                       }
                       else
                       {                        
                            if(this->mode_of_interest.compare("T")==0){
                                interest_per_installment=interest_rate/(no_of_payment_per_year*1000);
                            }

                            this->interest_index = (this->interest_index-1)*(this->loan_period_in_month/12)+1;
                            //total_payable_amount=round(loan_amount*((this->interest_index-1)*(this->loan_period_in_month/12)+1));
                            total_payable_amount=round(loan_amount*this->interest_index);

                            installment_amnt=(loan_amount * interest_per_installment * pow((interest_per_installment + 1), number_of_installment)) / (pow((interest_per_installment + 1), number_of_installment) - 1);
                            //total_amnt= installment_amnt*number_of_installment;
                            //if(extra_amount>0)
                            installment_amnt=my_round(installment_amnt)+extra_amount;
                            //actual_number_of_installments=ceil(my_round((log10(installment_amnt/(installment_amnt-loan_amount * interest_per_installment))/log10(interest_per_installment + 1))));
                            if(enforce_number_of_installment==0)                        
                            actual_number_of_installments=ceil(total_payable_amount/installment_amnt);
                            //actual_number_of_installments=ceil(my_round((log10(installment_amnt/(installment_amnt-loan_amount * interest_per_installment))/log10(interest_per_installment + 1))));

                            //installment_amnt=total_payable_amount/(this->number_of_installment);
                            //installment_amnt=round(installment_amnt+this->extra_amount);
                            //actual_number_of_installments=this->number_of_installment;

                            total_amnt=total_payable_amount;
                       
                       }
                   }

              total_payable_amount=total_amnt;
              //std::cerr<<"In Loan Construction"<<total_payable_amount << "####" <<total_amnt<<" "<<installment_amount<<std::endl;
              //std::cerr<<this->id<<" "<<this->branch_id<<" "<<this->samity_id<<" "<<this->product_id<<" "<<this->member_id<<std::endl;
              schedule_datum.set_loan_amount(this->loan_amnt);
              schedule_datum.set_total_payable_amount(this->total_payable_amount);
              
              schedule_datum_hash.set_loan_amount(this->loan_amnt);
              schedule_datum_hash.set_total_payable_amount(this->total_payable_amount);
             
}


loan::loan(const loan& orig) {
    
    std::ofstream fout;
   fout.open("/var/www/microfin360/trunk/application/logs/microfin360/scheduler.log",std::ios::app);
   fout<<"Copy Loan ID: "<<orig.GetId()<<" Created\n";

    id=orig.GetId();
    customized_loan_no = orig.GetCustomized_loan_no();
    loan_application_no = orig.GetLoan_application_no();
    branch_id = orig.GetBranch_id();
    samity_id = orig.GetSamity_id();
    product_id = orig.GetProduct_id();
    member_id = orig.GetMember_id();
    purpose_id = orig.GetPurpose_id();
    funding_org_id = orig.GetFunding_org_id();
    interest_calculation_method = orig.GetInterest_calculation_method();
    decline_period = orig.GetDecline_Period();
    loan_amount = orig.GetLoan_amount();
    interest_rate = orig.GetInterest_rate();
    interest_amount = orig.GetInterest_amount();
    discount_interest_amount = orig.GetDiscount_interest_amount();
    insurance_guarantor_percentage = orig.GetInsurance_guarantor_percentage();
    insurance_guarantor_amount = orig.GetInsurance_guarantor_amount();
    total_payable_amount = orig.GetTotal_payable_amount();
    number_of_installment = orig.GetNumber_of_installment();
    installment_amount = orig.GetInstallment_amount();
    repayment_frequency = orig.GetRepayment_frequency();
    grace_period = orig.GetGrace_period();
    loan_period_in_month = orig.GetLoan_period_in_month();
    mode_of_interest = orig.GetMode_of_interest();
    interest_rate_calculation_amount = orig.GetInterest_rate_calculation_amount();
    cycle = orig.GetCycle();
    first_repayment_date = orig.GetFirst_repayment_date();
    last_repayment_date = orig.GetLast_repayment_date();
    authorization_status = orig.GetAuthorization_status();
    disburse_registration_no = orig.GetDisburse_registration_no();
    disbursed_by = orig.GetDisbursed_by();
    disburse_date = orig.GetDisburse_date();
    authorized_by = orig.GetAuthorized_by();
    authorized_date = orig.GetAuthorized_date();
    ledger_id = orig.GetLedger_id();
    voucher_status = orig.GetVoucher_status();
    fully_paid_registration_no = orig.GetFully_paid_registration_no();
    overdue_registration_no = orig.GetOverdue_registration_no();
    loan_fully_paid_date = orig.GetLoan_fully_paid_date();
    current_status = orig.GetCurrent_status();
    loan_closing_date = orig.GetLoan_closing_date();
    loan_closing_info_verified_by = orig.GetLoan_closing_info_verified_by();
    is_deleted = orig.GetIs_deleted();

    is_authorized=orig.GetIs_authorized();

    is_loan_fully_paid=orig.GetIs_loan_fully_paid();
    total_amnt=orig.total_amnt; //= (loan_amount+(loan_period_in_month/12)*(interest_rate/100)*loan_amount);
    installment_amnt=orig.installment_amnt; //=total_amnt/number_of_installment;
    pri_installment_amnt=orig.pri_installment_amnt; //=loan_amount/number_of_installment;
    ins_installment_amnt=orig.ins_installment_amnt;
    no_of_payment_per_year=orig.no_of_payment_per_year; // = (number_of_installment*12)/loan_period_in_month;
    interest_per_installment=orig.interest_per_installment; //=interest_rate/(no_of_payment_per_year*100);

    schedule=orig.schedule;
    schedule_datum=orig.schedule_datum;
    schedule_datum_hash=orig.schedule_datum_hash;
    
    actual_number_of_installments=orig.actual_number_of_installments;
    skip_last_week_calculation_monthly=orig.skip_last_week_calculation_monthly;
    service_charge_taken_time=orig.GetService_charge_taken_time();
    
   //std::cerr<<"In Loan Copy Constructor\n";
    ////exit(1);
}

loan::~loan() {
    
       std::ofstream fout;
       fout.open("/var/www/microfin360/trunk/application/logs/microfin360/scheduler.log",std::ios::app);
       fout<<"Loan ID: "<<id<<" Destroyed\n";

	schedule.clear();
	schedule_datum.clear();
	schedule_datum_hash.clear();
        
        this->loan_transfer_daychange__info.clear();
        this->loan_transfer_samitychange__info.clear();
        this->one_time_weekly_loans_transactions__info.clear();
        week_days.clear();


}

int loan::GetAuthorization_status() const {
    return authorization_status;
}

void loan::SetAuthorization_status(int authorization_status) {
    this->authorization_status = authorization_status;
}

int loan::GetIs_authorized() const {
    return is_authorized;
}

void loan::SetIs_authorized(int is_authorized) {
    this->is_authorized = is_authorized;
}

std::string loan::GetAuthorized_by() const {
    return authorized_by;
}

void loan::SetAuthorized_by(std::string authorized_by) {
    this->authorized_by = authorized_by;
}

boost::gregorian::date loan::GetAuthorized_date() const {
    return authorized_date;
}

void loan::SetAuthorized_date(boost::gregorian::date authorized_date) {
    this->authorized_date = authorized_date;
}

int loan::GetBranch_id() const {
    return branch_id;
}

void loan::SetBranch_id(int branch_id) {
    this->branch_id = branch_id;
}

int loan::GetCurrent_status() const {
    return current_status;
}

void loan::SetCurrent_status(int current_status) {
    this->current_status = current_status;
}

std::string loan::GetCustomized_loan_no() const {
    return customized_loan_no;
}

void loan::SetCustomized_loan_no(std::string customized_loan_no) {
    this->customized_loan_no = customized_loan_no;
}

int loan::GetCycle() const {
    return cycle;
}

void loan::SetCycle(int cycle) {
    this->cycle = cycle;
}
int loan::Get_skip_last_week_calculation_monthly() {
    return skip_last_week_calculation_monthly;
}

void loan::Set_skip_last_week_calculation_monthly(int skip_last_week_calculation_monthly) {
    this->skip_last_week_calculation_monthly = skip_last_week_calculation_monthly;
}

boost::gregorian::date loan::GetDisburse_date() const {
    return disburse_date;
}

void loan::SetDisburse_date(boost::gregorian::date disburse_date) {
    this->disburse_date = disburse_date;
}
boost::gregorian::date loan::GetTransfer_in_date() const {
    return transfer_in_date;
}
void loan::SetTransfer_in_date(boost::gregorian::date transfer_in_date) {
    this->transfer_in_date = transfer_in_date;
}
boost::gregorian::date loan::GetTransfer_out_date() const {
    return transfer_out_date;
}
void loan::SetTransfer_out_date(boost::gregorian::date transfer_out_date) {
    this->transfer_out_date = transfer_out_date;
}

int loan::GetDisburse_registration_no() const {
    return disburse_registration_no;
}

void loan::SetDisburse_registration_no(int disburse_registration_no) {
    this->disburse_registration_no = disburse_registration_no;
}

std::string loan::GetDisbursed_by() const {
    return disbursed_by;
}

void loan::SetDisbursed_by(std::string disbursed_by) {
    this->disbursed_by = disbursed_by;
}

double loan::GetDiscount_interest_amount() const {
    return discount_interest_amount;
}

void loan::SetDiscount_interest_amount(double discount_interest_amount) {
    this->discount_interest_amount = discount_interest_amount;
}

boost::gregorian::date loan::GetFirst_repayment_date(bool actual) const {
    //return first_repayment_date;
    if(this->schedule.size()>0 && actual==true)
        return this->schedule[0];
    else
        return first_repayment_date;
}

void loan::SetFirst_repayment_date(boost::gregorian::date first_repayment_date) {
    this->first_repayment_date = first_repayment_date;
}

int loan::GetFully_paid_registration_no() const {
    return fully_paid_registration_no;
}

void loan::SetFully_paid_registration_no(int fully_paid_registration_no) {
    this->fully_paid_registration_no = fully_paid_registration_no;
}

int loan::GetFunding_org_id() const {
    return funding_org_id;
}

void loan::SetFunding_org_id(int funding_org_id) {
    this->funding_org_id = funding_org_id;
}

int loan::GetId() const {
    return id;
}

void loan::SetId(int id) {
    this->id = id;
}

double loan::GetInstallment_amount() const {
    return installment_amount;
}

void loan::SetInstallment_amount() {

    if (this->GetInterest_calculation_method().compare("F") == 0) {
        double interest_amount = (this->GetLoan_amount() * this->GetInterest_rate() * this->GetLoan_period_in_month()) / 12;
        double total_payable = this->GetLoan_amount() + interest_amount;
        this->installment_amount = floor(total_payable / this->GetNumber_of_installment());
    }
}

double loan::GetInsurance_guarantor_amount() const {
    return insurance_guarantor_amount;
}

void loan::SetInsurance_guarantor_amount(double insurance_guarantor_amount) {
    this->insurance_guarantor_amount = insurance_guarantor_amount;
}

double loan::GetInsurance_guarantor_percentage() const {
    return insurance_guarantor_percentage;
}

void loan::SetInsurance_guarantor_percentage(double insurance_guarantor_percentage) {
    this->insurance_guarantor_percentage = insurance_guarantor_percentage;
}

double loan::GetInterest_amount() const {
    return interest_amount;
}

void loan::SetInterest_amount(double interest_amount) {
    this->interest_amount = interest_amount;
}

std::string loan::GetInterest_calculation_method() const {
    return interest_calculation_method;
}

void loan::SetInterest_calculation_method(std::string interest_calculation_method) {
    this->interest_calculation_method = interest_calculation_method;
}

std::string loan::GetDecline_Period() const {
    return decline_period;
}

void loan::SetDecline_Period(std::string decline_period) {
    this->decline_period = decline_period;
}

double loan::GetInterest_rate() const {
    return interest_rate;
}

void loan::SetInterest_rate(double interest_rate) {
    this->interest_rate = interest_rate;
}

int loan::GetInterest_rate_calculation_amount() const {
    return interest_rate_calculation_amount;
}

void loan::SetInterest_rate_calculation_amount(int interest_rate_calculation_amount) {
    this->interest_rate_calculation_amount = interest_rate_calculation_amount;
}

int loan::GetIs_deleted() const {
    return is_deleted;
}

void loan::SetIs_deleted(int is_deleted) {
    this->is_deleted = is_deleted;
}

boost::gregorian::date loan::GetLast_repayment_date() const {
    return last_repayment_date;
}

void loan::SetLast_repayment_date(boost::gregorian::date last_repayment_date=boost::gregorian::from_simple_string("1900-01-01")) {
    this->last_repayment_date = last_repayment_date;
}

int loan::GetLedger_id() const {
    return ledger_id;
}

void loan::SetLedger_id(int ledger_id) {
    this->ledger_id = ledger_id;
}

double loan::GetLoan_amount() const {
    return loan_amount;
}

void loan::SetLoan_amount(double loan_amount) {
    this->loan_amount = loan_amount;
}

std::string loan::GetLoan_application_no() const {
    return loan_application_no;
}

void loan::SetLoan_application_no(std::string loan_application_no) {
    this->loan_application_no = loan_application_no;
}

boost::gregorian::date loan::GetLoan_closing_date() const {
    return loan_closing_date;
}

void loan::SetLoan_closing_date(boost::gregorian::date loan_closing_date) {
    this->loan_closing_date = loan_closing_date;
}

int loan::GetLoan_closing_info_verified_by() const {
    return loan_closing_info_verified_by;
}

void loan::SetLoan_closing_info_verified_by(int loan_closing_info_verified_by) {
    this->loan_closing_info_verified_by = loan_closing_info_verified_by;
}

boost::gregorian::date loan::GetLoan_fully_paid_date() const {
    return loan_fully_paid_date;
}

void loan::SetLoan_fully_paid_date(boost::gregorian::date loan_fully_paid_date) {
    this->loan_fully_paid_date = loan_fully_paid_date;
}

double loan::GetLoan_period_in_month() const {
    return loan_period_in_month;
}

void loan::SetLoan_period_in_month(double loan_period_in_month) {
    this->loan_period_in_month = loan_period_in_month;
}

int loan::GetMember_id() const {
    return member_id;
}

void loan::SetMember_id(int member_id) {
    this->member_id = member_id;
}

std::string loan::GetMode_of_interest() const {
    return mode_of_interest;
}

void loan::SetMode_of_interest(std::string mode_of_interest) {
    this->mode_of_interest = mode_of_interest;
}

int loan::GetNumber_of_installment() const {
    return number_of_installment;
}

int loan::GetActualNumber_of_installment() const {
    return this->actual_number_of_installments;
}

void loan::SetNumber_of_installment(int number_of_installment) {
    this->number_of_installment = number_of_installment;
    //std::cout<<" "<<number_of_installment<<std::endl;
}

int loan::GetOverdue_registration_no() const {
    return overdue_registration_no;
}

void loan::SetOverdue_registration_no(int overdue_registration_no) {
    this->overdue_registration_no = overdue_registration_no;
}

int loan::GetProduct_id() const {
    return product_id;
}

void loan::SetProduct_id(int product_id) {
    this->product_id = product_id;
}

int loan::GetPurpose_id() const {
    return purpose_id;
}

void loan::SetPurpose_id(int purpose_id) {
    this->purpose_id = purpose_id;
}

std::string loan::GetRepayment_frequency() const {
    return repayment_frequency;
}

void loan::SetRepayment_frequency(std::string repayment_frequency) {
    this->repayment_frequency = repayment_frequency;
}

std::string loan::GetGrace_period() const {
    return grace_period;
}

void loan::SetGrace_period(std::string grace_period) {
    this->grace_period = grace_period;
}

int loan::GetSamity_id() const {
    return samity_id;
}

void loan::SetSamity_id(int samity_id) {
    this->samity_id = samity_id;
}

double loan::GetTotal_payable_amount() const {
    return total_payable_amount;
}

void loan::SetTotal_payable_amount() {
/*
    if (this->GetInterest_calculation_method().compare("F") == 0) {
        double interest_amount = (this->GetLoan_amount() * this->GetInterest_rate() * this->GetLoan_period_in_month()) / 12;
        double total_payable = this->GetLoan_amount() + interest_amount;
        this->total_payable_amount = total_payable;
    }
    else if (this->GetInterest_calculation_method().compare("D") == 0) {
        double interest_amount = (this->GetLoan_amount() * this->GetInterest_rate() * this->GetLoan_period_in_month()) / 12;
        double total_payable = this->GetLoan_amount() + interest_amount;
        this->total_payable_amount = total_payable;
    }
 */
}

int loan::GetVoucher_status() const {
    return voucher_status;
}

void loan::SetVoucher_status(int voucher_status) {
    this->voucher_status = voucher_status;
}

void loan::SetIs_loan_fully_paid(int is_loan_fully_paid){
	//std::cerr<<std::endl<<"In set fully paid"<<std::endl;
	this->is_loan_fully_paid=is_loan_fully_paid;
}

int loan::GetIs_loan_fully_paid() const{
	return is_loan_fully_paid;
}

void loan::addNewSchedule_date(boost::gregorian::date d_date) {
    //std::cerr<<"Total Payable Amount"<<schedule_datum.size()<<" "<<total_payable_amount<<std::endl;
    schedule.push_back(d_date);
    
	int s_d_size=schedule_datum.size();
	if(s_d_size>0 && s_d_size+1<number_of_installment){
	total_payable_amount=total_payable_amount+schedule_datum[0].installment_amount;
//	schedule_datum.push_back(schedule_data(d_date,schedule_datum[0].installment_amount,schedule_datum[0].principle_installment_amount,schedule_datum[0].interrest_installment_amount));
//	schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,schedule_datum[0].installment_amount,schedule_datum[0].principle_installment_amount,schedule_datum[0].interrest_installment_amount)));
	return;
	}

	double installment_amount;
	double principle_installment_amount;

	calculate_Instalment_amounts(schedule_datum.size()+1,installment_amount,principle_installment_amount);
	double interrest_installment_amount=installment_amount-principle_installment_amount;

	//std::cerr<<"back from In calculation"<<std::endl;
	if(schedule_datum.size()==0){
		first_repayment_date=d_date;
		total_payable_amount=0;
		//std::cerr<<"Yahoo:";
	}
	else if(schedule_datum.size()+1==number_of_installment){
		last_repayment_date=d_date;
		//std::cerr<<"Yahoo:";
	}
	total_payable_amount=total_payable_amount+installment_amount;

    //std::cerr<<"Yahoo:";

    //std::cerr<<"Yahoo:"<<installment_amount<<" "<<principle_installment_amount<<" "<<interrest_installment_amount<<std::endl;

//	schedule_datum.push_back(schedule_data(d_date,installment_amount,principle_installment_amount,interrest_installment_amount));

//	schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,installment_amount,principle_installment_amount,interrest_installment_amount)));

	//std::cerr<<number_of_installment<<" "<<installment_amount<<" "<<principle_installment_amount<<" "<<interrest_installment_amount<<std::endl;
	//if((schedule_datum.size()==number_of_installment) && id==801)
	////exit(1);
}



void loan::addNewOneTimeSchedule_date(boost::gregorian::date& d_date){

    this->actual_number_of_installments=this->number_of_installment;
    boost::gregorian::date_duration d_D=(d_date-this->disburse_date);
    int no_of_D=d_D.days();
    boost::gregorian::date_duration d_DD=(this->first_repayment_date-this->disburse_date);
    if(no_of_D<d_DD.days()){
        no_of_D=d_DD.days();
    }

    total_amnt=loan_amnt;//+loan_amnt*no_of_D*(this->interest_rate/1000);

    if(this->mode_of_interest.compare("H")==0){
        total_amnt=loan_amnt;//+loan_amnt*no_of_D*(this->interest_rate/100);
    }

    real_principle_installment_amount=loan_amnt;
    real_interrest_installment_amount=total_amnt-loan_amnt;
    real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
    real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
    
    schedule_data temp_schedule_data(d_date,total_amnt,loan_amnt,total_amnt-loan_amnt,real_principle_installment_amount,real_interrest_installment_amount);
    schedule.push_back(d_date);
    schedule_datum.push_back(temp_schedule_data);
    schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

    /*
    schedule.push_back(d_date);
    schedule_datum.push_back(schedule_data(d_date,total_amnt,loan_amnt,total_amnt-loan_amnt));
    schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,total_amnt,loan_amnt,total_amnt-loan_amnt)));
    */ 
    updateFirstLastInstallmentNumber();

}
void loan::addNewOneTimeSchedule_date_multiple(boost::gregorian::date& d_date){

    int installment_number=schedule_datum.size()+1;    
    this->actual_number_of_installments=this->number_of_installment;   
    
     if(installment_number==this->actual_number_of_installments){
          total_amnt=loan_amnt-round(loan_amnt/this->actual_number_of_installments)* (this->actual_number_of_installments-1);
          if(this->mode_of_interest.compare("H")==0){
             total_amnt=loan_amnt-round(loan_amnt/this->actual_number_of_installments)* (this->actual_number_of_installments-1);
          }         
          real_principle_installment_amount=loan_amnt- round(loan_amnt/this->actual_number_of_installments)* (this->actual_number_of_installments-1);
     }else{         
        total_amnt=round(loan_amnt/this->actual_number_of_installments);
        if(this->mode_of_interest.compare("H")==0){
            total_amnt=round(loan_amnt/this->actual_number_of_installments);
        }
        real_principle_installment_amount=round(loan_amnt/this->actual_number_of_installments);
     }
    real_interrest_installment_amount=total_amnt-round(loan_amnt/this->actual_number_of_installments);
    real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
    real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
    
    schedule_data temp_schedule_data(d_date,total_amnt,loan_amnt,total_amnt-loan_amnt,real_principle_installment_amount,real_interrest_installment_amount);
    schedule.push_back(d_date);
    schedule_datum.push_back(temp_schedule_data);
    schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));    
    updateFirstLastInstallmentNumber();
}

void loan::addNewFLATSchedule_date(boost::gregorian::date& d_date) {

//std::cerr<<"Total Payable Amount"<<schedule_datum.size()<<" "<<total_payable_amount<<std::endl;
  int installment_number=schedule_datum.size()+1;
            //std::cerr<<installment_number<<" "<<actual_number_of_installments<<" "<<d_date<<std::endl;
            if(installment_number==this->actual_number_of_installments)
                this->addLastFLATSchedule_date(d_date);
            else if(installment_number<this->actual_number_of_installments){
    //std::cerr<<"In flat schedule "<<d_date<<std::endl;
    
    real_principle_installment_amount=(((total_installment_amount+installment_amnt)/this->interest_index)-((total_installment_amount)/this->interest_index));
    real_interrest_installment_amount=(installment_amnt-real_principle_installment_amount);
    real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
    real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;


    schedule_data temp_schedule_data(d_date,installment_amnt,pri_installment_amnt,ins_installment_amnt,real_principle_installment_amount,real_interrest_installment_amount);
    schedule.push_back(d_date);
    schedule_datum.push_back(temp_schedule_data);
    schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

    /*
    schedule.push_back(d_date);
    schedule_datum.push_back(schedule_data(d_date, installment_amnt, pri_installment_amnt, ins_installment_amnt));
    //boost::gregorian::to_iso_extended_string(d_date);
    schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date, installment_amnt, pri_installment_amnt, ins_installment_amnt)));
    */      }

}

void loan::addNewDFDOneTimeSchedule_dateDailyBasis(boost::gregorian::date& d_date) {      
        
        boost::gregorian::date_duration d_D=(d_date-this->disburse_date);
        int no_of_D=d_D.days();       
        if(this->loan_period_in_day > 1 && this->is_loan_period_in_day_fixed ==1){
            no_of_D=this->loan_period_in_day;           
        }
        
        
        
        total_amnt=round(loan_amnt+(loan_amnt*no_of_D*this->interest_index)/1000);

        if(this->mode_of_interest.compare("H")==0){
            total_amnt=round(loan_amnt+(loan_amnt*no_of_D*this->interest_index)/100);
        }

        real_principle_installment_amount=loan_amnt;
        real_interrest_installment_amount=total_amnt-loan_amnt;
        real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
        real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
        this->total_payable_amount=real_total_principle_amount+real_total_interrest_amount;
        schedule_data temp_schedule_data(d_date,total_amnt,loan_amnt,total_amnt-loan_amnt,real_principle_installment_amount,real_interrest_installment_amount);
        schedule.push_back(d_date);
        schedule_datum.push_back(temp_schedule_data);
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

        /*
        schedule.push_back(d_date);
        schedule_datum.push_back(schedule_data(d_date,total_amnt,loan_amnt,total_amnt-loan_amnt));
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,total_amnt,loan_amnt,total_amnt-loan_amnt)));
        */ 
        updateFirstLastInstallmentNumber();
       

}

void loan::addNewOneTimeFLATSchedule_date(boost::gregorian::date& d_date, bool is_round){
    
            //std::cerr<<"MURAD ASM FIRST BEGIN:"<< d_date << std::endl;
            if(d_date<=this->disburse_date){
                return;
            }
            boost::gregorian::date_duration d_D=(d_date-this->disburse_date);
            if(schedule.size()>0){ 
                d_D=(d_date-schedule[schedule.size()-1]);
            }
            int no_of_D=d_D.days();

            one_time_weekly_priciple_paid_amount=0;
            one_time_weekly_priciple_paid_amount_murad=0;

            double one_time_weekly_priciple_paid_amount_current=0;
            double one_time_weekly_priciple_paid_amount_local=0;
            int curr_payment_flag=0;
            int curr_payment_flag_murad=0;
            boost::gregorian::date prev_schedule_date_murad_local;
            boost::gregorian::date prev_transaction_date_murad_local;

            for(__gnu_cxx::hash_map<boost::gregorian::date, double, boost::hash<boost::gregorian::date> >::iterator it1=one_time_weekly_loans_transactions__info.begin();it1!=one_time_weekly_loans_transactions__info.end();it1++)
            {

                if(it1->first<=d_date){
                    
                    //std::cerr<< it1->first << " - "<< it1->second << std::endl; 

                    
                    //std::cerr<< "one_time_weekly_priciple_paid_amount " << " - "<< one_time_weekly_priciple_paid_amount << std::endl; 
                    if(prev_schedule_date_murad_temp.is_not_a_date()){
                          prev_schedule_date_murad_temp=this->disburse_date;
                    }
                    if(prev_schedule_date_murad_temp < it1->first && it1->first < d_date){
                       
                        //one_time_weekly_priciple_paid_amount_murad =it1->second;
                        one_time_weekly_priciple_paid_amount_local=it1->second;
                        one_time_weekly_priciple_paid_amount_current+=it1->second;
                        prev_schedule_date_murad_local=prev_schedule_date_murad_temp;
                        prev_transaction_date_murad_local=it1->first;
                        schedule_date_murad=d_date;
                        curr_payment_flag_murad=1;
                        one_time_weekly_priciple_paid_amount_murad =one_time_weekly_priciple_paid_amount_current;
                        //std::cerr<<"one_time_:" <<one_time_weekly_priciple_paid_amount_current << std::endl;
                    }                    
                    else if(it1->first==d_date){
                        //std::cerr<<"one_time_:" <<one_time_weekly_priciple_paid_amount_current << std::endl;
                        if(one_time_weekly_priciple_paid_amount_current==0){
                        one_time_weekly_priciple_paid_amount_current+=it1->second;
                        }
                        //one_time_weekly_priciple_paid_amount+=it1->second;
                        //one_time_weekly_priciple_paid_amount_current+=it1->second;
                        curr_payment_flag=1;
                        //if(curr_payment_flag_murad==1){
                        one_time_weekly_priciple_paid_amount_murad=it1->second+one_time_weekly_priciple_paid_amount_current;

                        //}
                     }  
                    //std::cerr<<"First :" <<it1->first <<" Date " <<d_date << std::endl;
                    if(it1->first<d_date){
                        one_time_weekly_priciple_paid_amount+=it1->second;
                    }
                    

                }
                /*
                else if(it1->first==d_date){
                    //one_time_weekly_priciple_paid_amount+=it1->second;
                    one_time_weekly_priciple_paid_amount_current+=it1->second;
                    curr_payment_flag=1;
                    if(curr_payment_flag_murad==1){
                    one_time_weekly_priciple_paid_amount_murad=one_time_weekly_priciple_paid_amount_murad+one_time_weekly_priciple_paid_amount_current;

                    }
                }*/
            }

            double interest_amt=(loan_amnt-one_time_weekly_priciple_paid_amount)*no_of_D*(this->interest_rate/1000);

            //double interest_amt=loan_amnt*no_of_D*(this->interest_rate/1000);

            if(this->mode_of_interest.compare("H")==0){
                //interest_amt=loan_amnt*no_of_D*(this->interest_rate/100);
                interest_amt=(loan_amnt-one_time_weekly_priciple_paid_amount)*no_of_D*(this->interest_rate/100);
            }
            //std::cerr<< " Interest Amount " << interest_amt <<"Principal Amount "<<one_time_weekly_priciple_paid_amount <<std::cerr;
            if(curr_payment_flag_murad==1){
                boost::gregorian::date_duration d_D_murad=(prev_transaction_date_murad_local-prev_schedule_date_murad_temp);
                int no_of_D_murad=d_D_murad.days();
                interest_amt=(loan_amnt-one_time_weekly_priciple_paid_amount + one_time_weekly_priciple_paid_amount_local )*no_of_D_murad*(this->interest_rate/1000);

                interest_amt=interest_amt+(loan_amnt-one_time_weekly_priciple_paid_amount_current)*(no_of_D-no_of_D_murad)*(this->interest_rate/1000);

            }
            
            if(is_round){
                interest_amt=round(interest_amt);
            }
            //interest_amt =ceil(interest_amt);

            //real_principle_installment_amount=0;

            if(curr_payment_flag==1 && curr_payment_flag_murad!=1){
            //real_principle_installment_amount=one_time_weekly_priciple_paid_amount;
                real_principle_installment_amount=one_time_weekly_priciple_paid_amount_current;               
            }
            else if(curr_payment_flag==1 && curr_payment_flag_murad==1){
                real_principle_installment_amount=one_time_weekly_priciple_paid_amount_murad;                
            ////std::cerr<<"one_time_weekly_priciple_paid_amount_murad:" <<one_time_weekly_priciple_paid_amount_murad << std::endl;
            }
            else if(curr_payment_flag!=1 && curr_payment_flag_murad==1){
                real_principle_installment_amount=one_time_weekly_priciple_paid_amount_murad;                  
            }
            else{
                real_principle_installment_amount=0;          
            
            }

            
            interest_amt=round(interest_amt); 

            real_interrest_installment_amount=interest_amt;
            real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
            real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
            //std::cerr<<"real_principle_installment_amount: " <<real_principle_installment_amount<<" real_interrest_installment_amount:" <<real_interrest_installment_amount<<" real_total_principle_amount:" << real_total_principle_amount <<" real_total_interrest_amount:"<< real_total_interrest_amount<< "Interest Amount : "<<interest_amt<<std::endl;

            //schedule_data temp_schedule_data(d_date,interest_amt,0,interest_amt,real_principle_installment_amount,real_interrest_installment_amount);
            
            schedule_data temp_schedule_data(d_date,real_principle_installment_amount+real_interrest_installment_amount,real_principle_installment_amount,interest_amt,real_principle_installment_amount,real_interrest_installment_amount);
            schedule.push_back(d_date);
            schedule_datum.push_back(temp_schedule_data);
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

            /*
            schedule.push_back(d_date);
            schedule_datum.push_back(schedule_data(d_date,interest_amt,0,interest_amt));
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,interest_amt,0,interest_amt)));
            */
            updateFirstLastInstallmentNumber();
            prev_schedule_date_murad_temp=d_date;
            ////std::cerr<<"MURAD ASM FIRST END:\n" ;
}


        void loan::addNewOneTimeFLATSchedule_dateMonthly(boost::gregorian::date& d_date, bool is_round){ 

        
        if(d_date<=this->disburse_date)
          return;
        boost::gregorian::date_duration d_D=(d_date-this->disburse_date);
        if(schedule.size()>0){ 
          d_D=(d_date-schedule[schedule.size()-1]);
        }
        //int no_of_D=d_D.days();
        int no_of_D=d_D.days();
        int no_of_M=0;        
        if(no_of_D>30){
         no_of_M=no_of_D/30;
         no_of_M=round(no_of_M); 
        }else{
         no_of_M=1; 
        }
        //int no_of_M=1;
        one_time_weekly_priciple_paid_amount=0;
        one_time_weekly_priciple_paid_amount_murad=0;

        int curr_payment_flag_murad=0;
        boost::gregorian::date prev_schedule_date_murad_local;
        boost::gregorian::date prev_transaction_date_murad_local;
        double interest_amt=0.00;
        if(prev_schedule_date_murad_temp.is_not_a_date()){
           prev_schedule_date_murad_temp=this->disburse_date;
        }
        for(__gnu_cxx::hash_map<boost::gregorian::date, double, boost::hash<boost::gregorian::date> >::iterator it1=one_time_weekly_loans_transactions__info.begin();it1!=one_time_weekly_loans_transactions__info.end();it1++){

            if(it1->first<d_date){
                one_time_weekly_priciple_paid_amount+=it1->second;
                
                if(prev_schedule_date_murad_temp < it1->first && it1->first < d_date){
                    one_time_weekly_priciple_paid_amount_murad+=it1->second;                   
                    prev_schedule_date_murad_local=prev_schedule_date_murad_temp;
                    prev_transaction_date_murad_local=it1->first;
                    schedule_date_murad=d_date;
                    curr_payment_flag_murad=1;
                } 
            }
            if(it1->first==d_date){
                one_time_weekly_priciple_paid_amount+=it1->second;
                one_time_weekly_priciple_paid_amount_murad+=it1->second;                
            }
        }

        interest_amt=(loan_amnt-(one_time_weekly_priciple_paid_amount-one_time_weekly_priciple_paid_amount_murad))*no_of_M*(this->interest_rate/1000); 
        //std::cerr<< "interest_amt:"<< interest_amt << std::endl;
        if(this->mode_of_interest.compare("H")==0){
          interest_amt=(loan_amnt-(one_time_weekly_priciple_paid_amount-one_time_weekly_priciple_paid_amount_murad))*no_of_M*(this->interest_rate/100);
        } 
           
        
           boost::gregorian::date_duration d_D_murad=(prev_transaction_date_murad_local-prev_schedule_date_murad_temp);
           int no_of_D_murad=d_D_murad.days();
           no_of_D_murad=no_of_D_murad/30;     
           if(prev_schedule_date_murad_temp < prev_transaction_date_murad_local && prev_transaction_date_murad_local < d_date){
              interest_amt=(loan_amnt-one_time_weekly_priciple_paid_amount + one_time_weekly_priciple_paid_amount_murad)*(no_of_M-no_of_D_murad)*(this->interest_rate/1000);
              if(this->mode_of_interest.compare("H")==0){
                interest_amt=(loan_amnt-one_time_weekly_priciple_paid_amount + one_time_weekly_priciple_paid_amount_murad)*(no_of_M-no_of_D_murad)*(this->interest_rate/100);
              } 
              //std::cerr<< "interest_amt 1:"<< interest_amt << std::endl;
            }   
        
        
        if(is_round)
           interest_amt=round(interest_amt);
        
        real_principle_installment_amount=one_time_weekly_priciple_paid_amount_murad;
        
               

        real_interrest_installment_amount=interest_amt;
        real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
        real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
        schedule_data temp_schedule_data(d_date,real_principle_installment_amount+real_interrest_installment_amount,real_principle_installment_amount,interest_amt,real_principle_installment_amount,real_interrest_installment_amount);
        schedule.push_back(d_date);
        schedule_datum.push_back(temp_schedule_data);
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

        updateFirstLastInstallmentNumber();
        prev_schedule_date_murad_temp=d_date;
        ////std::cerr<<"MURAD ASM FIRST END:\n" ;
        }


        void loan::addLastOneTimeFLATSchedule_date(boost::gregorian::date& d_date){


            boost::gregorian::date_duration d_D=(d_date-this->disburse_date);
            //boost::gregorian::date_duration d_D=(this->first_repayment_date-d_date);
            //d_date=this->first_repayment_date;
            if(schedule.size()>0){
                d_D=(d_date-schedule[schedule.size()-1]);
            }
            int no_of_D=d_D.days();
            //std::cerr<<no_of_D<<std::endl;
            //exit(1);
            one_time_weekly_priciple_paid_amount=0;
            for(__gnu_cxx::hash_map<boost::gregorian::date, double, boost::hash<boost::gregorian::date> >::iterator it1=one_time_weekly_loans_transactions__info.begin();it1!=one_time_weekly_loans_transactions__info.end();it1++){
                if(it1->first<=d_date){
                    one_time_weekly_priciple_paid_amount+=it1->second;
                }
            }
            double interest_amt=(loan_amnt-one_time_weekly_priciple_paid_amount)*no_of_D*(this->interest_rate/1000);

            //double interest_amt=loan_amnt*no_of_D*(this->interest_rate/1000);

            if(this->mode_of_interest.compare("H")==0){
                //interest_amt=loan_amnt*no_of_D*(this->interest_rate/100);
                interest_amt=(loan_amnt-one_time_weekly_priciple_paid_amount)*no_of_D*(this->interest_rate/100);
            }
            //interest_amt =ceil(interest_amt);


            interest_amt=round(real_total_interrest_amount+interest_amt)-real_total_interrest_amount;
            interest_amt=round(interest_amt);
            real_principle_installment_amount=this->loan_amnt-one_time_weekly_priciple_paid_amount;
            //real_principle_installment_amount=this->loan_amnt;
            real_interrest_installment_amount=interest_amt;
            real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
            real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;

            if(real_principle_installment_amount+real_interrest_installment_amount<0.001)
                return;
            schedule_data temp_schedule_data(d_date,real_principle_installment_amount+interest_amt,real_principle_installment_amount,interest_amt,real_principle_installment_amount,real_interrest_installment_amount);
            schedule.push_back(d_date);
            schedule_datum.push_back(temp_schedule_data);
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

            this->total_amnt=real_total_principle_amount+real_total_interrest_amount;
            this->total_payable_amount=real_total_principle_amount+real_total_interrest_amount;
            this->actual_number_of_installments=schedule_datum.size();
            //this->total_amnt=this->loan_amount;
            //this->total_payable_amount=this->loan_amount;
            /*
            schedule.push_back(d_date);
            schedule_datum.push_back(schedule_data(d_date,interest_amt,0,interest_amt));
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,interest_amt,0,interest_amt)));
            */
            updateFirstLastInstallmentNumber();
        }


        void loan::addLastOneTimeFLATSchedule_dateMonthly(boost::gregorian::date& d_date){

        boost::gregorian::date_duration d_D=(d_date-this->disburse_date);
        //boost::gregorian:ate_duration d_D=(this->first_repayment_date-d_date);
        //d_date=this->first_repayment_date;
        if(schedule.size()>0){
        d_D=(d_date-schedule[schedule.size()-1]);
        }
        int no_of_D=d_D.days(); 
        int no_of_M=ceil(no_of_D/30);        
        //exit(1);
        one_time_weekly_priciple_paid_amount=0;
        for(__gnu_cxx::hash_map<boost::gregorian::date, double, boost::hash<boost::gregorian::date> >::iterator it1=one_time_weekly_loans_transactions__info.begin();it1!=one_time_weekly_loans_transactions__info.end();it1++){
            if(it1->first<=d_date){
            one_time_weekly_priciple_paid_amount+=it1->second;
            }
        }
        double interest_amt=0.00;
        if((*this->system_date) == d_date){
             interest_amt=(loan_amnt-one_time_weekly_priciple_paid_amount)*no_of_M*(this->interest_rate/1000);
       
            if(this->mode_of_interest.compare("H")==0){
            //interest_amt=loan_amnt*no_of_D*(this->interest_rate/100);
            interest_amt=(loan_amnt-one_time_weekly_priciple_paid_amount)*no_of_M*(this->interest_rate/100);
            }
        }      

        interest_amt=round(real_total_interrest_amount+interest_amt)-real_total_interrest_amount;
        interest_amt=round(interest_amt);        
        real_principle_installment_amount=this->loan_amnt-one_time_weekly_priciple_paid_amount;
        //real_principle_installment_amount=this->loan_amnt;
        real_interrest_installment_amount=interest_amt;
        real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
        real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;

        if(real_principle_installment_amount+real_interrest_installment_amount<0.001)
        return;
        schedule_data temp_schedule_data(d_date,real_principle_installment_amount+interest_amt,real_principle_installment_amount,interest_amt,real_principle_installment_amount,real_interrest_installment_amount);
        schedule.push_back(d_date);
        schedule_datum.push_back(temp_schedule_data);
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

        this->total_amnt=real_total_principle_amount+real_total_interrest_amount;
        this->total_payable_amount=real_total_principle_amount+real_total_interrest_amount;
        this->actual_number_of_installments=schedule_datum.size();
        
        this->last_repayment_date=d_date;
        
        updateFirstLastInstallmentNumber();
        }


void loan::addNewMonthlyPricipleFirstSchedule_date(boost::gregorian::date& d_date){
    
    //std::cerr<<"In addNewMonthlyPricipleFirstSchedule_date\n";
    ins_installment_amnt=0;
    pri_installment_amnt=loan_amnt/(this->actual_number_of_installments-1);
    
    char buffer[100];
    sprintf(buffer,"%.2f",pri_installment_amnt);
    pri_installment_amnt=strtod(buffer,NULL);
    installment_amnt=pri_installment_amnt+ins_installment_amnt;
    if(schedule.size()+1==this->actual_number_of_installments){
        /*
        pri_installment_amnt=loan_amnt-pri_installment_amnt*(this->actual_number_of_installments-2);
        sprintf(buffer,"%.2f",pri_installment_amnt);
        pri_installment_amnt=strtod(buffer,NULL);
        */ 
        this->addNewMonthlyPricipleLastSchedule_date(d_date);
        return;
    }
    
    //std::cerr<<"Date "<<d_date<<" pri_installment_amnt "<<pri_installment_amnt<<" ins_installment_amnt "<<ins_installment_amnt<<"\n";
    real_principle_installment_amount=pri_installment_amnt;
    real_interrest_installment_amount=ins_installment_amnt;
    real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
    real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
            
    
    schedule_data temp_schedule_data(d_date,installment_amnt,pri_installment_amnt,ins_installment_amnt,real_principle_installment_amount,real_interrest_installment_amount);
    schedule.push_back(d_date);
    schedule_datum.push_back(temp_schedule_data);
    schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

    /*
    schedule.push_back(d_date);
    schedule_datum.push_back(schedule_data(d_date,pri_installment_amnt,pri_installment_amnt,ins_installment_amnt));
    schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,pri_installment_amnt,pri_installment_amnt,ins_installment_amnt))); 
    */
    updateFirstLastInstallmentNumber();
}

void loan::addNewMonthlyPricipleLastSchedule_date(boost::gregorian::date& d_date){
    
    //std::cerr<<"In addNewMonthlyPricipleLastSchedule_date\n";
    if(schedule.size()+1==this->actual_number_of_installments){
        ins_installment_amnt=total_payable_amount-loan_amnt;
        pri_installment_amnt=0;
        installment_amnt=pri_installment_amnt+ins_installment_amnt;
        //std::cerr<<"Date "<<d_date<<" pri_installment_amnt "<<pri_installment_amnt<<" ins_installment_amnt "<<ins_installment_amnt<<"\n";
        real_principle_installment_amount=pri_installment_amnt;
        real_interrest_installment_amount=ins_installment_amnt;
        real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
        real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
            

        schedule_data temp_schedule_data(d_date,installment_amnt,pri_installment_amnt,ins_installment_amnt,real_principle_installment_amount,real_interrest_installment_amount);
        schedule.push_back(d_date);
        schedule_datum.push_back(temp_schedule_data);
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));
        /*
        schedule.push_back(d_date);
        schedule_datum.push_back(schedule_data(d_date,ins_installment_amnt,pri_installment_amnt,ins_installment_amnt));
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,ins_installment_amnt,pri_installment_amnt,ins_installment_amnt))); 
        */
    }
    updateFirstLastInstallmentNumber();
}

void loan::addNewDECLININGSchedule_date(boost::gregorian::date& d_date) {
//std::cerr<<"Total Payable Amount"<<schedule_datum.size()<<" "<<total_payable_amount<<std::endl;
//std::cerr<<"Total Priciple Amount"<<total_principle_amount<<" Total Interrest Amount "<<total_interrest_amount<<std::endl;
        
           
          int installment_number=schedule_datum.size()+1;
            //std::cerr<<installment_number<<" "<<actual_number_of_installments<<" "<<d_date<<std::endl;
            if(installment_number==this->actual_number_of_installments)
                this->addLastDECLININGSchedule_date(d_date);
            else if(installment_number<this->actual_number_of_installments){

                ins_installment_amnt=(loan_amnt*interest_per_installment*pow(interest_per_installment+1,installment_number-1))-(installment_amnt*pow(interest_per_installment+1,installment_number-1))+installment_amnt;
                ins_installment_amnt=my_round4(ins_installment_amnt);
                pri_installment_amnt=installment_amnt-ins_installment_amnt;
                
                pri_installment_amnt=my_round4(pri_installment_amnt);
                ins_installment_amnt=my_round4(ins_installment_amnt);
                
                real_principle_installment_amount=(pri_installment_amnt);
                real_interrest_installment_amount=(ins_installment_amnt);
                real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
                real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
                
            
                
                schedule_data temp_schedule_data(d_date,my_round4(installment_amnt),my_round4(pri_installment_amnt),my_round4(ins_installment_amnt),real_principle_installment_amount,real_interrest_installment_amount);
                schedule.push_back(d_date);
                schedule_datum.push_back(temp_schedule_data);
                schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

                //total_principle_amount=total_principle_amount+my_round(pri_installment_amnt);
                //total_interrest_amount=total_interrest_amount+my_round(ins_installment_amnt);
                
                total_installment_amount=total_installment_amount+my_round4(installment_amnt);
                total_principle_amount=total_principle_amount+my_round4(pri_installment_amnt);
                total_interrest_amount=total_interrest_amount+my_round4(ins_installment_amnt);
                
                total_installment_amount=my_round4(total_installment_amount);
                total_principle_amount=my_round4(total_principle_amount);
                total_interrest_amount=my_round4(total_interrest_amount);
                //std::cerr<<"Total Priciple Amount"<<total_principle_amount<<" Total Interrest Amount "<<total_interrest_amount<<std::endl;
                //std::cerr<<"Installment Amnt"<<installment_amnt<<" Pri Installment Amnt "<<pri_installment_amnt<<" Ins Installment Amnt "<<ins_installment_amnt<<std::endl;
                //std::cerr<<"Installment Amnt"<<my_round(installment_amnt)<<" Pri Installment Amnt "<<my_round(pri_installment_amnt)<<" Ins Installment Amnt "<<my_round(ins_installment_amnt)<<std::endl;
                
                /*
                schedule.push_back(d_date);
                schedule_datum.push_back(schedule_data(d_date,installment_amnt,pri_installment_amnt,ins_installment_amnt));
                schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,installment_amnt,pri_installment_amnt,ins_installment_amnt)));

                total_principle_amount=total_principle_amount+pri_installment_amnt;
                total_interrest_amount=total_interrest_amount+total_interrest_amount;
                */
            }


}

    void loan::addNewDECLININGSchedule_date_daily_basis(boost::gregorian::date& d_date) {
    //std::cerr<<"Total Payable Amount "<<schedule_datum.size()<<" "<<total_payable_amount<<std::endl;
    //std::cerr<<"Total Priciple Amount "<<total_principle_amount<<" Total Interrest Amount "<<total_interrest_amount<<std::endl;
    //std::cerr<<"total_interrest_amount "<<total_interrest_amount<<" "<<std::endl;
    int installment_number=schedule_datum.size()+1; 
    if(installment_number==this->actual_number_of_installments)
    this->addLastDECLININGSchedule_date(d_date);
    else if(installment_number<this->actual_number_of_installments){ 
    //this->actual_number_of_installments=this->number_of_installment;
    int no_of_D=0; 
    if(total_payable_amount-loan_amount>= total_interrest_amount ){ 
        //std::cerr<<"Schedule till interest finished : "<<schedule_datum.size() << "###"<<total_interrest_amount<<std::endl;
        if(installment_number==1){
            boost::gregorian::date_duration d_D=(d_date-this->disburse_date); 
            no_of_D=d_D.days()+1;
            ins_installment_amnt = round((((this->interest_rate/100)*(this->loan_amnt))/((360*this->loan_period_in_month)/12))*no_of_D) ;
        }else{
            boost::gregorian::date_duration d_D=(d_date-this->prev_repayment_date);
            no_of_D=d_D.days();
            ins_installment_amnt = round((((this->interest_rate/100)*(this->loan_amnt-total_principle_amount))/((360*this->loan_period_in_month)/12))*no_of_D) ;
        
            
            if((this->loan_amnt-total_principle_amount)<0){
                ins_installment_amnt=installment_amnt;            
            }
            else if(((this->loan_amnt-total_principle_amount)- (installment_amnt-ins_installment_amnt))< 0 ){
                //ins_installment_amnt=installment_amnt-(this->loan_amnt-total_principle_amount); 
                ins_installment_amnt= installment_amnt - ( this->loan_amnt-total_principle_amount);
            }
            /*
            if((ins_installment_amnt+real_total_interrest_amount)>(this->total_payable_amount-this->loan_amount)){
                double extra_amount=(ins_installment_amnt+real_total_interrest_amount)-(this->total_payable_amount-this->loan_amount);
                ins_installment_amnt-=extra_amount;
            }
            if(ins_installment_amnt<0){
                ins_installment_amnt=0;
            }
          */
        }     
        if((total_payable_amount-loan_amount)< (total_interrest_amount +ins_installment_amnt)){
        //ins_installment_amnt=round(ins_installment_amnt-((total_interrest_amount +ins_installment_amnt)- (total_payable_amount-loan_amount)));
            if(ceil(total_payable_amount-loan_amount) > total_interrest_amount){            
                ins_installment_amnt= ceil(total_payable_amount-loan_amount)- total_interrest_amount;
            }
            else{         
            
            ins_installment_amnt=round((total_payable_amount-loan_amount)-total_interrest_amount);
            
            }
               // std::cerr<< total_payable_amount-loan_amount << " Here  : " <<ins_installment_amnt +total_interrest_amount  <<std::endl;
        } 
    } 
    else{       
        ins_installment_amnt = 0;
    }
    //ins_installment_amnt=(loan_amnt*interest_per_installment*pow(interest_per_installment+1,installment_number-1))-(installment_amnt*pow(interest_per_installment+1,installment_number-1))+installment_amnt;
    if(installment_amnt < ins_installment_amnt){
        ins_installment_amnt= installment_amnt;
    }
       
    ins_installment_amnt=my_round4(ins_installment_amnt);
    pri_installment_amnt=installment_amnt-ins_installment_amnt;   
      

    pri_installment_amnt=my_round4(pri_installment_amnt);
    ins_installment_amnt=my_round4(ins_installment_amnt);

    real_principle_installment_amount=(pri_installment_amnt);
    real_interrest_installment_amount=(ins_installment_amnt);
    real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
    real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
    //std::cerr << "Real Total Interest " << real_total_interrest_amount <<std::endl;
    this->prev_repayment_date=d_date;

    schedule_data temp_schedule_data(d_date,my_round4(installment_amnt),my_round4(pri_installment_amnt),my_round4(ins_installment_amnt),real_principle_installment_amount,real_interrest_installment_amount);
    schedule.push_back(d_date);
    schedule_datum.push_back(temp_schedule_data);
    schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

    total_installment_amount=total_installment_amount+my_round4(installment_amnt);
    total_principle_amount=total_principle_amount+my_round4(pri_installment_amnt);
    total_interrest_amount=total_interrest_amount+my_round4(ins_installment_amnt);

    total_installment_amount=my_round4(total_installment_amount);
    total_principle_amount=my_round4(total_principle_amount);
    total_interrest_amount=my_round4(total_interrest_amount);
    //std::cerr<<"Total Priciple Amount"<<total_principle_amount<<" Total Interrest Amount "<<total_interrest_amount<<std::endl;
    //std::cerr<<"Installment Amnt"<<installment_amnt<<" Pri Installment Amnt "<<pri_installment_amnt<<" Ins Installment Amnt "<<ins_installment_amnt<<std::endl;
    //std::cerr<<"Installment Amnt"<<my_round(installment_amnt)<<" Pri Installment Amnt "<<my_round(pri_installment_amnt)<<" Ins Installment Amnt "<<my_round(ins_installment_amnt)<<std::endl;
    }


    }





void loan::addNewDFDSchedule_date(boost::gregorian::date& d_date) {
//std::cerr<<"Total Payable Amount"<<schedule_datum.size()<<" "<<total_payable_amount<<std::endl;
           
        int installment_number=schedule_datum.size()+1;
        //std::cerr<<installment_number<<" "<<actual_number_of_installments<<" "<<d_date<<std::endl;
        if(installment_number==this->actual_number_of_installments)
            this->addLastDFDSchedule_date(d_date);
        else if(installment_number<this->actual_number_of_installments){
            pri_installment_amnt=installment_amnt/this->interest_index;
            ins_installment_amnt=installment_amnt-pri_installment_amnt;
            installment_amnt=pri_installment_amnt+ins_installment_amnt;
           
            real_principle_installment_amount=((total_installment_amount+installment_amnt)/this->interest_index)-((total_installment_amount)/this->interest_index);
            real_interrest_installment_amount=installment_amnt-real_principle_installment_amount;
            real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
            real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
            
            schedule_data temp_schedule_data(d_date,installment_amnt,pri_installment_amnt,ins_installment_amnt,real_principle_installment_amount,real_interrest_installment_amount);
            schedule.push_back(d_date);
            schedule_datum.push_back(temp_schedule_data);
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

            total_installment_amount=total_installment_amount+installment_amnt;
            total_principle_amount=total_principle_amount+pri_installment_amnt;
            total_interrest_amount=total_interrest_amount+ins_installment_amnt;
        }
}

void loan::addLastDFDSchedule_date(boost::gregorian::date& d_date) {

//std::cerr<<"Total Payable Amount"<<schedule_datum.size()<<" "<<total_payable_amount<<std::endl;
   
    //std::cerr<<"In Last DFD schedule "<<d_date<<std::endl;

    int installment_number=schedule_datum.size()+1;
    if(installment_number==this->actual_number_of_installments){


        //ins_installment_amnt=(loan_amnt*interest_per_installment*pow(interest_per_installment+1,installment_number-1))-(installment_amnt*pow(interest_per_installment+1,installment_number-1))+installment_amnt;
        //pri_installment_amnt=installment_amnt-ins_installment_amnt;

        //pri_installment_amnt=my_round(loan_amount-total_principle_amount);
        //ins_installment_amnt=my_round(this->total_payable_amount-loan_amount-total_interrest_amount);
        
        pri_installment_amnt=loan_amount-total_principle_amount;
        ins_installment_amnt=this->total_payable_amount-loan_amount-total_interrest_amount;

        installment_amnt=pri_installment_amnt+ins_installment_amnt;
        installment_amnt=my_round(installment_amnt);
        ins_installment_amnt=installment_amnt-pri_installment_amnt;
        
        real_principle_installment_amount=loan_amount-real_total_principle_amount;
        real_interrest_installment_amount=this->total_payable_amount-loan_amount-real_total_interrest_amount;
        real_interrest_installment_amount=installment_amnt-real_principle_installment_amount;
        real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
        real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
            
        
        if(this->actual_number_of_installments!=1){
            this->first_repayment_date =schedule[0];
        }
        this->last_repayment_date=d_date;

        //total_principle_amount=total_principle_amount+my_round(pri_installment_amnt);
        //total_interrest_amount=total_interrest_amount+my_round(ins_installment_amnt);
        
        total_principle_amount=total_principle_amount+pri_installment_amnt;
        total_interrest_amount=total_interrest_amount+ins_installment_amnt;

        //schedule.push_back(d_date);
        //schedule_datum.push_back(schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt)));
        //schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt))));
        
        schedule_data temp_schedule_data(d_date,installment_amnt,pri_installment_amnt,ins_installment_amnt,real_principle_installment_amount,real_interrest_installment_amount);
        schedule.push_back(d_date);
        schedule_datum.push_back(temp_schedule_data);
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

        /*
        schedule.push_back(d_date);
        schedule_datum.push_back(schedule_data(d_date,installment_amnt,pri_installment_amnt,ins_installment_amnt));
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,installment_amnt,pri_installment_amnt,ins_installment_amnt)));
        */
        
        total_payable_amount=total_principle_amount+total_interrest_amount;
        //std::cerr<<"Total Payable "<<total_payable_amount<<std::endl;
        this->first_repayment_date =schedule[0];
        this->last_repayment_date=d_date;
       
    }
    updateFirstLastInstallmentNumber();
}
void loan::addLastDFDSchedule_date_enforce(boost::gregorian::date& d_date, boost::gregorian::date& d_date_first, int enforce_number = 0) {

    int installment_number = schedule_datum.size() + 1;
    if (installment_number == this->actual_number_of_installments) {

        pri_installment_amnt = loan_amount - total_principle_amount;
        ins_installment_amnt = this->total_payable_amount - loan_amount - total_interrest_amount;

        installment_amnt = pri_installment_amnt + ins_installment_amnt;
        installment_amnt = my_round(installment_amnt);
        ins_installment_amnt = installment_amnt - pri_installment_amnt;
        
        if(this->repayment_mode_for_monthly_loan=="LISC"){
            pri_installment_amnt=0;
            ins_installment_amnt=installment_amnt;
            real_principle_installment_amount=0;
            real_interrest_installment_amount=installment_amnt;
        }
        real_principle_installment_amount = loan_amount - real_total_principle_amount;
        real_interrest_installment_amount = this->total_payable_amount - loan_amount - real_total_interrest_amount;
        real_interrest_installment_amount = installment_amnt - real_principle_installment_amount;
        real_total_principle_amount = real_total_principle_amount + real_principle_installment_amount;
        real_total_interrest_amount = real_total_interrest_amount + real_interrest_installment_amount;


        this->first_repayment_date = schedule[0];
        this->last_repayment_date = d_date;

        total_principle_amount = total_principle_amount + pri_installment_amnt;
        total_interrest_amount = total_interrest_amount + ins_installment_amnt;

        if (enforce_number > 0) {
            double temp_installment_amount = schedule_datum.sch_data[enforce_number - 1].installment_amount;
            double principle_installment_amount = schedule_datum.sch_data[enforce_number - 1].principle_installment_amount;
            double temp_interrest_installment_amount = schedule_datum.sch_data[enforce_number - 1].interrest_installment_amount;
            double temp_real_principle_installment_amount = schedule_datum.sch_data[enforce_number - 1].real_principle_installment_amount;
            double temp_real_interrest_installment_amount = schedule_datum.sch_data[enforce_number - 1].real_interrest_installment_amount;

            schedule_datum.sch_data[enforce_number - 1].installment_amount = installment_amnt;
            schedule_datum.sch_data[enforce_number - 1].principle_installment_amount = pri_installment_amnt;
            schedule_datum.sch_data[enforce_number - 1].interrest_installment_amount = ins_installment_amnt;
            schedule_datum.sch_data[enforce_number - 1].real_principle_installment_amount = real_principle_installment_amount;
            schedule_datum.sch_data[enforce_number - 1].real_interrest_installment_amount = real_interrest_installment_amount;

            schedule_data temp_schedule_data(d_date, temp_installment_amount, principle_installment_amount, temp_interrest_installment_amount, temp_real_principle_installment_amount, temp_real_interrest_installment_amount);
            schedule.push_back(d_date);
            schedule_datum.push_back(temp_schedule_data);
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));            
            /* if(this->repayment_mode_for_monthly_loan=="LISC"){
                boost::gregorian::date temp_schedule_date=schedule_datum.sch_data[enforce_number - 1].schedule_date;
                schedule_datum.clear();
                schedule_datum_hash.clear();
                schedule.clear();
                
                schedule_data temp_schedule_data1(temp_schedule_date, installment_amnt, pri_installment_amnt, ins_installment_amnt, real_principle_installment_amount, real_interrest_installment_amount);
                schedule.push_back(temp_schedule_date);
                schedule_datum.push_back(temp_schedule_data1);
                schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(temp_schedule_date), temp_schedule_data1));
                
                schedule_datum.sch_data[0].installment_amount = installment_amnt;
                schedule_datum.sch_data[0].principle_installment_amount = 0;
                schedule_datum.sch_data[0].interrest_installment_amount = installment_amnt;
                schedule_datum.sch_data[0].real_principle_installment_amount = 0;
                schedule_datum.sch_data[0].real_interrest_installment_amount = installment_amnt;
                
                this->total_payable_amount=this->total_payable_amount-installment_amnt;
                this->interest_index=this->total_payable_amount/this->loan_amount;
                
                this->installment_amnt=temp_installment_amount;
                                
                int schedule_count=1;
                while(++schedule_count<this->actual_number_of_installments){                  
                    this->addNewDFDSchedule_date(schedule[schedule_count]);
                }
                schedule_data temp_schedule_data(d_date, temp_installment_amount, principle_installment_amount, temp_interrest_installment_amount, temp_real_principle_installment_amount, temp_real_interrest_installment_amount);
                schedule.push_back(d_date);
                schedule_datum.push_back(temp_schedule_data);
                schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));
                
                schedule_datum.sch_data[this->actual_number_of_installments-1].installment_amount = temp_installment_amount;
                schedule_datum.sch_data[this->actual_number_of_installments-1].principle_installment_amount = temp_installment_amount/this->interest_index;
                schedule_datum.sch_data[this->actual_number_of_installments-1].interrest_installment_amount = temp_installment_amount-schedule_datum.sch_data[this->actual_number_of_installments-1].principle_installment_amount;
                schedule_datum.sch_data[this->actual_number_of_installments-1].real_principle_installment_amount =  schedule_datum.sch_data[this->actual_number_of_installments-1].principle_installment_amount;
                schedule_datum.sch_data[this->actual_number_of_installments-1].real_interrest_installment_amount = schedule_datum.sch_data[this->actual_number_of_installments-1].interrest_installment_amount;
                
                this->total_payable_amount=this->total_payable_amount+installment_amnt;
                this->interest_index=this->total_payable_amount/this->loan_amount;
            }*/
            //}
        } else {
            schedule_data temp_schedule_data(d_date, installment_amnt, pri_installment_amnt, ins_installment_amnt, real_principle_installment_amount, real_interrest_installment_amount);
            schedule.push_back(d_date);
            schedule_datum.push_back(temp_schedule_data);
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));
        }
       
        total_payable_amount = total_principle_amount + total_interrest_amount;
        //std::cerr<<"Total Payable "<<total_payable_amount<<std::endl;
        this->first_repayment_date = schedule[0];
        this->last_repayment_date = d_date;

    }
    updateFirstLastInstallmentNumber();
}

void loan::addNewASMSchedule_date(boost::gregorian::date& d_date) {
    
    int installment_number=schedule_datum.size()+1;
    
    if(installment_number==this->number_of_installment)
            this->addLastASMSchedule_date(d_date);
    else if(installment_number==1){
            pri_installment_amnt=round(this->loan_amnt/this->number_of_installment);
            total_principle_amount=this->total_principle_amount+pri_installment_amnt;
            ins_installment_amnt = round((this->interest_rate/100)*(this->loan_amnt));
            total_interrest_amount=total_interrest_amount+ins_installment_amnt;
            
            real_principle_installment_amount=pri_installment_amnt;
            real_interrest_installment_amount=ins_installment_amnt;
                    
            real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
            real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;                
            
            installment_amnt=pri_installment_amnt+ins_installment_amnt;
            
            schedule_data temp_schedule_data(d_date,installment_amnt,pri_installment_amnt,ins_installment_amnt,real_principle_installment_amount,real_interrest_installment_amount);
            schedule.push_back(d_date);
            schedule_datum.push_back(temp_schedule_data);
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

            /*
            schedule.push_back(d_date);
            schedule_datum.push_back(schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt)));
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt))));
            */ 
    }
    else{
            ins_installment_amnt = round((this->interest_rate/100)*(this->loan_amnt-this->total_principle_amount));
            total_principle_amount=round(this->total_principle_amount+pri_installment_amnt);
            
            total_interrest_amount=total_interrest_amount+ins_installment_amnt;
            installment_amnt=pri_installment_amnt+ins_installment_amnt;
           
            real_principle_installment_amount=(pri_installment_amnt);
            real_interrest_installment_amount=(ins_installment_amnt);
            
            real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
            real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;                
            
            
            schedule_data temp_schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt),real_principle_installment_amount,real_interrest_installment_amount);
            schedule.push_back(d_date);
            schedule_datum.push_back(temp_schedule_data);
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

            /*
            schedule.push_back(d_date);
            schedule_datum.push_back(schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt)));
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt))));
            */
    }
}
void loan::addLastASMSchedule_date(boost::gregorian::date& d_date) {
    
        pri_installment_amnt = this->loan_amnt-total_principle_amount;
        total_principle_amount=this->total_principle_amount+pri_installment_amnt;
        ins_installment_amnt = round((this->interest_rate/100)*(pri_installment_amnt));;
        total_interrest_amount=total_interrest_amount+ins_installment_amnt;

        installment_amnt=pri_installment_amnt+ins_installment_amnt;
        
        real_principle_installment_amount=(pri_installment_amnt);
        real_interrest_installment_amount=(ins_installment_amnt);

        real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
        real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;                


        schedule_data temp_schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt),real_principle_installment_amount,real_interrest_installment_amount);
        schedule.push_back(d_date);
        schedule_datum.push_back(temp_schedule_data);
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

        /*
        schedule.push_back(d_date);
        schedule_datum.push_back(schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt)));
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt))));
        */
        this->total_amnt=total_principle_amount+total_interrest_amount;
        this->total_payable_amount=total_principle_amount+total_interrest_amount;
        this->actual_number_of_installments=schedule_datum.size();
        
        updateFirstLastInstallmentNumber();
}
void loan::addNewEFRRAPSchedule_date(boost::gregorian::date& d_date) {
//std::cerr<<"EFRAP Total Payable Amount"<<schedule_datum.size()<<" "<<total_payable_amount<<std::endl;
           
        int installment_number=schedule_datum.size()+1;
        //std::cerr<<installment_number<<" "<<actual_number_of_installments<<" "<<d_date<<std::endl;
        if(installment_number==this->actual_number_of_installments)
            this->addLastEFRRAPSchedule_date(d_date);
        else if(installment_number==1){
            if(this->extra_amount>0)
                this->total_payable_amount=round(this->total_payable_amount);
            pri_installment_amnt=0;
            ins_installment_amnt=this->total_payable_amount-this->loan_amnt;
            installment_amnt=pri_installment_amnt+ins_installment_amnt;
           
            real_principle_installment_amount=my_round(pri_installment_amnt);
            real_interrest_installment_amount=my_round(ins_installment_amnt);
            
            real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
            real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
            
            //std::cerr<<"installment_amnt "<<installment_amnt<<" pri_installment_amnt "<<pri_installment_amnt<<" ins_installment_amnt "<<ins_installment_amnt<<std::endl;
            //std::cerr<<"installment_amnt "<<installment_amnt<<" pri_installment_amnt "<<pri_installment_amnt<<" ins_installment_amnt "<<ins_installment_amnt<<std::endl;
            schedule_data temp_schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt),real_principle_installment_amount,real_interrest_installment_amount);
            schedule.push_back(d_date);
            schedule_datum.push_back(temp_schedule_data);
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

            /*
            schedule.push_back(d_date);
            schedule_datum.push_back(schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt)));
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt))));
            */
            total_principle_amount=total_principle_amount+my_round(pri_installment_amnt);
            total_interrest_amount=total_interrest_amount+my_round(ins_installment_amnt);
        }
        else if(installment_number<this->actual_number_of_installments){

           
            //pri_installment_amnt=this->GetLoan_amount()/actual_number_of_installments;
            //ins_installment_amnt=installment_amnt-pri_installment_amnt;
            
            pri_installment_amnt=this->loan_amnt/(this->actual_number_of_installments-1)+this->extra_amount;
            ins_installment_amnt=0;
            installment_amnt=pri_installment_amnt+ins_installment_amnt;
           
            real_principle_installment_amount=my_round(pri_installment_amnt);
            real_interrest_installment_amount=my_round(ins_installment_amnt);
            
            real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
            real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
            
            schedule_data temp_schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt),real_principle_installment_amount,real_interrest_installment_amount);
            schedule.push_back(d_date);
            schedule_datum.push_back(temp_schedule_data);
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

            /*
            schedule.push_back(d_date);
            schedule_datum.push_back(schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt)));
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt))));
            */
            total_principle_amount=total_principle_amount+my_round(pri_installment_amnt);
            total_interrest_amount=total_interrest_amount+my_round(ins_installment_amnt);

        }
}

void loan::addNewEFRRAPSchedule_date_new(boost::gregorian::date& d_date) {
//std::cerr<<"EFRAP Total Payable Amount"<<schedule_datum.size()<<" "<<total_payable_amount<<std::endl;
           
        int installment_number=schedule_datum.size()+1;
        //std::cerr<<installment_number<<" "<<actual_number_of_installments<<" "<<d_date<<std::endl;
        if(installment_number==this->actual_number_of_installments)
            this->addLastEFRRAPSchedule_date(d_date);
        
        else if(installment_number<this->actual_number_of_installments){

           
            //pri_installment_amnt=this->GetLoan_amount()/actual_number_of_installments;
            //ins_installment_amnt=installment_amnt-pri_installment_amnt;
            
            pri_installment_amnt= 0;
            ins_installment_amnt= (this->total_payable_amount-this->loan_amnt)/(this->actual_number_of_installments)+this->extra_amount;
           
            installment_amnt=pri_installment_amnt+ins_installment_amnt;
           
            real_principle_installment_amount=my_round(pri_installment_amnt);
            real_interrest_installment_amount=my_round(ins_installment_amnt);
            
            real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
            real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
            
            schedule_data temp_schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt),real_principle_installment_amount,real_interrest_installment_amount);
            schedule.push_back(d_date);
            schedule_datum.push_back(temp_schedule_data);
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

            /*
            schedule.push_back(d_date);
            schedule_datum.push_back(schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt)));
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt))));
            */
            total_principle_amount=total_principle_amount+my_round(pri_installment_amnt);
            total_interrest_amount=total_interrest_amount+my_round(ins_installment_amnt);

        }
}
void loan::addNewSCTBISchedule_dateDFD(boost::gregorian::date& d_date) {
        //std::cerr<<"EFRAP Total Payable Amount"<<schedule_datum.size()<<" "<<total_payable_amount<<std::endl;
           
        int installment_number=schedule_datum.size()+1;
        //std::cerr<<installment_number<<" ## "<<actual_number_of_installments<<" "<<d_date<<std::endl;
        if(installment_number>=this->actual_number_of_installments)
            this->addLastSCTBISchedule_dateDFD(d_date);
        
        else if(installment_number<this->actual_number_of_installments){

          // std::cerr<<"total_payable_amount"<<" "<<this->total_payable_amount<<" "<<d_date<<std::endl;
            //pri_installment_amnt=this->GetLoan_amount()/actual_number_of_installments;
            //ins_installment_amnt=installment_amnt-pri_installment_amnt;
            
            pri_installment_amnt= 0;
            ins_installment_amnt= this->installment_amountdb;
           
            installment_amnt=pri_installment_amnt+ins_installment_amnt;
           
            real_principle_installment_amount=my_round(pri_installment_amnt);
            real_interrest_installment_amount=my_round(ins_installment_amnt);
            
            real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
            real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
            //total_payable_amount=real_total_principle_amount+real_total_interrest_amount;
            schedule_data temp_schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt),real_principle_installment_amount,real_interrest_installment_amount);
            schedule.push_back(d_date);
            schedule_datum.push_back(temp_schedule_data);
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

            /*
            schedule.push_back(d_date);
            schedule_datum.push_back(schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt)));
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt))));
            */
            total_principle_amount=total_principle_amount+my_round(pri_installment_amnt);
            total_interrest_amount=total_interrest_amount+my_round(ins_installment_amnt);
            //total_payable_amount=total_principle_amount+total_interrest_amount;

        }
}
void loan::addLastSCTBISchedule_dateDFD(boost::gregorian::date& d_date) {

     //std::cerr<<"Total Payable Amount"<<schedule_datum.size()<<" "<<total_payable_amount<<std::endl;
   
    //std::cerr<<"In Last schedule "<<d_date<<std::endl;
    //std::cerr<<"In Last schedule 1st repay "<<this->GetFirst_repayment_date()<<std::endl;

        int installment_number=schedule_datum.size()+1;
        if(installment_number==this->actual_number_of_installments){
        
       
        pri_installment_amnt=this->loan_amnt;
        ins_installment_amnt=(this->total_payable_amount-this->loan_amnt)-total_interrest_amount;

        installment_amnt=pri_installment_amnt+ins_installment_amnt;
         //std::cerr<<"ins_installment_amnt "<<ins_installment_amnt<<std::endl;
        
        
        this->first_repayment_date =schedule[0];
        this->last_repayment_date=d_date;

        total_principle_amount=total_principle_amount+my_round(pri_installment_amnt);
        total_interrest_amount=total_interrest_amount+my_round(ins_installment_amnt);

        real_total_principle_amount=my_round(pri_installment_amnt);
        real_total_interrest_amount=my_round(ins_installment_amnt);
        
        real_principle_installment_amount=this->loan_amnt;        
        real_interrest_installment_amount=my_round(ins_installment_amnt);
        
        
        real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
        real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
        
        schedule_data temp_schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt),real_principle_installment_amount,real_interrest_installment_amount);
        schedule.push_back(d_date);
        schedule_datum.push_back(temp_schedule_data);
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

        total_payable_amount=total_principle_amount+total_interrest_amount;
        //std::cerr<<"Total Payable "<<total_payable_amount<<std::endl;
        this->first_repayment_date =schedule[0];
        this->last_repayment_date=d_date;
       
    }
    updateFirstLastInstallmentNumber();
   
        
 }


void loan::addLastEFRRAPSchedule_date(boost::gregorian::date& d_date) {

//std::cerr<<"Total Payable Amount"<<schedule_datum.size()<<" "<<total_payable_amount<<std::endl;
   
    //std::cerr<<"In Last Efrap schedule "<<d_date<<std::endl;

    int installment_number=schedule_datum.size()+1;
    if(installment_number==this->actual_number_of_installments){


        //ins_installment_amnt=(loan_amnt*interest_per_installment*pow(interest_per_installment+1,installment_number-1))-(installment_amnt*pow(interest_per_installment+1,installment_number-1))+installment_amnt;
        //pri_installment_amnt=installment_amnt-ins_installment_amnt;
 
        pri_installment_amnt=loan_amount-total_principle_amount;
        ins_installment_amnt=0;

        installment_amnt=pri_installment_amnt+ins_installment_amnt;


        this->first_repayment_date =schedule[0];
        this->last_repayment_date=d_date;

        total_principle_amount=total_principle_amount+my_round(pri_installment_amnt);
        total_interrest_amount=total_interrest_amount+my_round(ins_installment_amnt);

        real_total_principle_amount=my_round(pri_installment_amnt);
        real_total_interrest_amount=my_round(ins_installment_amnt);

        real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
        real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;

        schedule_data temp_schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt),real_principle_installment_amount,real_interrest_installment_amount);
        schedule.push_back(d_date);
        schedule_datum.push_back(temp_schedule_data);
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

        /*
        schedule.push_back(d_date);
        schedule_datum.push_back(schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt)));
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt))));
        */
        total_payable_amount=total_principle_amount+total_interrest_amount;
        //std::cerr<<"Total Payable "<<total_payable_amount<<std::endl;
        this->first_repayment_date =schedule[0];
        this->last_repayment_date=d_date;
       
    }
    updateFirstLastInstallmentNumber();
}
void loan::addLastEFRRAPSchedule_date_new(boost::gregorian::date& d_date) {

//std::cerr<<"Total Payable Amount"<<schedule_datum.size()<<" "<<total_payable_amount<<std::endl;
   
    //std::cerr<<"In Last Efrap schedule "<<d_date<<std::endl;

    int installment_number=schedule_datum.size()+1;
    if(installment_number==this->actual_number_of_installments){


        //ins_installment_amnt=(loan_amnt*interest_per_installment*pow(interest_per_installment+1,installment_number-1))-(installment_amnt*pow(interest_per_installment+1,installment_number-1))+installment_amnt;
        //pri_installment_amnt=installment_amnt-ins_installment_amnt;
        //this->total_payable_amount-this->loan_amnt
        pri_installment_amnt=this->loan_amnt;
        ins_installment_amnt=(this->total_payable_amount-this->loan_amnt)/this->actual_number_of_installments;

        installment_amnt=pri_installment_amnt+ins_installment_amnt;

        
        
        this->first_repayment_date =schedule[0];
        this->last_repayment_date=d_date;

        total_principle_amount=total_principle_amount+my_round(pri_installment_amnt);
        total_interrest_amount=total_interrest_amount+my_round(ins_installment_amnt);

        real_total_principle_amount=my_round(pri_installment_amnt);
        real_total_interrest_amount=my_round(ins_installment_amnt);
        
        real_principle_installment_amount=this->loan_amnt;
        
        real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
        real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
        
        //std::cerr<<"pri_installment_amnt "<<pri_installment_amnt<<std::endl;
        //std::cerr<<"real_principle_installment_amount "<<real_principle_installment_amount<<std::endl;
      
        
        schedule_data temp_schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt),real_principle_installment_amount,real_interrest_installment_amount);
        schedule.push_back(d_date);
        schedule_datum.push_back(temp_schedule_data);
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

        /*
        schedule.push_back(d_date);
        schedule_datum.push_back(schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt)));
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt))));
        */
        total_payable_amount=total_principle_amount+total_interrest_amount;
        //std::cerr<<"Total Payable "<<total_payable_amount<<std::endl;
        this->first_repayment_date =schedule[0];
        this->last_repayment_date=d_date;
       
    }
    updateFirstLastInstallmentNumber();
}

void loan::addLastFLATSchedule_date(boost::gregorian::date& d_date) {

//std::cerr<<"Total Payable Amount"<<schedule_datum.size()<<" "<<total_payable_amount<<std::endl;
   
    int installment_number=schedule_datum.size()+1;
    if(installment_number==this->actual_number_of_installments){
        //std::cerr<<"In Last flat schedule "<<d_date<<std::endl;
        //installment_amnt=total_amnt-installment_amnt*(actual_number_of_installments-1);
        pri_installment_amnt=my_round(loan_amount-pri_installment_amnt*(actual_number_of_installments-1));
        //ins_installment_amnt=installment_amnt-pri_installment_amnt;
        ins_installment_amnt=my_round((total_amnt-loan_amount)-ins_installment_amnt*(actual_number_of_installments-1));
        installment_amnt=pri_installment_amnt+ins_installment_amnt;
        installment_amnt=ceil(my_trim(installment_amnt));
        ins_installment_amnt=installment_amnt-pri_installment_amnt;
        
        real_principle_installment_amount=(loan_amount-real_total_principle_amount);
        real_interrest_installment_amount=installment_amnt-real_principle_installment_amount;
        real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
        real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
            

        schedule_data temp_schedule_data(d_date,installment_amnt,pri_installment_amnt,ins_installment_amnt,real_principle_installment_amount,real_interrest_installment_amount);
        schedule.push_back(d_date);
        schedule_datum.push_back(temp_schedule_data);
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

        /*
        schedule.push_back(d_date);
        schedule_datum.push_back(schedule_data(d_date, installment_amnt, pri_installment_amnt, ins_installment_amnt));
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date, installment_amnt, pri_installment_amnt, ins_installment_amnt)));
        */ 
        this->first_repayment_date =schedule[0];
        this->last_repayment_date=d_date;
    }
    updateFirstLastInstallmentNumber();
}

void loan::addLastFLATSchedule_date_enforce(boost::gregorian::date& d_date, boost::gregorian::date& d_date_first, int enforce_number = 0) {

    int installment_number = schedule_datum.size() + 1;
    if (installment_number == this->actual_number_of_installments) {

         pri_installment_amnt=my_round(loan_amount-pri_installment_amnt*(actual_number_of_installments-1));
        //ins_installment_amnt=installment_amnt-pri_installment_amnt;
        ins_installment_amnt=my_round((total_amnt-loan_amount)-ins_installment_amnt*(actual_number_of_installments-1));
        installment_amnt=pri_installment_amnt+ins_installment_amnt;
        installment_amnt=ceil(my_trim(installment_amnt));
        ins_installment_amnt=installment_amnt-pri_installment_amnt;
               
        real_principle_installment_amount=(loan_amount-real_total_principle_amount);
        real_interrest_installment_amount=installment_amnt-real_principle_installment_amount;
        real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
        real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;
                      

        this->first_repayment_date = schedule[0];
        this->last_repayment_date = d_date;

        total_principle_amount = total_principle_amount + pri_installment_amnt;
        total_interrest_amount = total_interrest_amount + ins_installment_amnt;
        //std::cerr<<" total_principle_amount: "<< total_principle_amount<<" total_interrest_amount: "<<total_interrest_amount<< std::endl;
        if (enforce_number > 0) {
            double temp_installment_amount = schedule_datum.sch_data[enforce_number - 1].installment_amount;
            double principle_installment_amount = schedule_datum.sch_data[enforce_number - 1].principle_installment_amount;
            double temp_interrest_installment_amount = schedule_datum.sch_data[enforce_number - 1].interrest_installment_amount;
            double temp_real_principle_installment_amount = schedule_datum.sch_data[enforce_number - 1].real_principle_installment_amount;
            double temp_real_interrest_installment_amount = schedule_datum.sch_data[enforce_number - 1].real_interrest_installment_amount;

            schedule_datum.sch_data[enforce_number - 1].installment_amount = installment_amnt;
            schedule_datum.sch_data[enforce_number - 1].principle_installment_amount = real_principle_installment_amount;
            schedule_datum.sch_data[enforce_number - 1].interrest_installment_amount = real_interrest_installment_amount;
            schedule_datum.sch_data[enforce_number - 1].real_principle_installment_amount = real_principle_installment_amount;
            schedule_datum.sch_data[enforce_number - 1].real_interrest_installment_amount = real_interrest_installment_amount;

            schedule_data temp_schedule_data(d_date, temp_installment_amount, principle_installment_amount, temp_interrest_installment_amount, temp_real_principle_installment_amount, temp_real_interrest_installment_amount);
            schedule.push_back(d_date);
            schedule_datum.push_back(temp_schedule_data);
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));            
            total_payable_amount = real_total_principle_amount + real_total_interrest_amount;    
        } else {
            //std::cerr<< " After installment_amnt: "<< installment_amnt <<" pri_installment_amnt : "<< pri_installment_amnt<<" ins_installment_amnt: "<<ins_installment_amnt<<" real_principle_installment_amount: "<<real_principle_installment_amount<<" real_interrest_installment_amount: "<<real_interrest_installment_amount<<  std::endl;
       
            schedule_data temp_schedule_data(d_date, installment_amnt, pri_installment_amnt, ins_installment_amnt, real_principle_installment_amount, real_interrest_installment_amount);
            schedule.push_back(d_date);
            schedule_datum.push_back(temp_schedule_data);
            schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));
            total_payable_amount = total_principle_amount + total_interrest_amount;     
        
        }
       
        
        //std::cerr<<"Total Payable "<<total_payable_amount<<std::endl;
        this->first_repayment_date = schedule[0];
        this->last_repayment_date = d_date;

    }
    updateFirstLastInstallmentNumber();
    
    
    
    
    
    
}


void loan::addLastDECLININGSchedule_date(boost::gregorian::date& d_date) {

//std::cerr<<"Total Payable Amount"<<schedule_datum.size()<<" "<<total_payable_amount<<std::endl;
//std::cerr<<"Total Priciple Amount"<<total_principle_amount<<" Total Interrest Amount "<<total_interrest_amount<<std::endl;
        
   
    //std::cerr<<"In Last Declining schedule "<<d_date<<std::endl;
    //std::cerr<<" Loan Amount "<<loan_amount<<std::endl;

    int installment_number=schedule_datum.size()+1;
    if(installment_number==this->actual_number_of_installments){

        //total_principle_amount = my_round(total_principle_amount);
        //std::cerr<<"loan Amount "<<loan_amount<<" Total Priciple Amount"<<total_principle_amount<<" Pri Installment Amnt "<<pri_installment_amnt<<std::endl;
        pri_installment_amnt=loan_amount-total_principle_amount;
        //std::cerr<<"loan Amount "<<loan_amount<<" Total Priciple Amount"<<total_principle_amount<<" Pri Installment Amnt "<<pri_installment_amnt<<std::endl;
        //loan_amount=pri_installment_amnt+total_principle_amount;
        //std::cerr<<"loan Amount "<<loan_amount<<" Total Priciple Amount"<<total_principle_amount<<" Pri Installment Amnt "<<pri_installment_amnt<<std::endl;
        
        //std::cerr<<"interest_per_installment "<<interest_per_installment<<std::endl;
        
        
        //ins_installment_amnt=pri_installment_amnt*interest_per_installment;
        //ins_installment_amnt=my_round(pri_installment_amnt*interest_per_installment);
        ins_installment_amnt=my_round(total_payable_amount-loan_amount-total_interrest_amount);

        installment_amnt=my_round(pri_installment_amnt+ins_installment_amnt);
        //std::cerr<<"Installment Amnt"<<installment_amnt<<" Pri Installment Amnt "<<pri_installment_amnt<<" Ins Installment Amnt "<<ins_installment_amnt<<std::endl;
        //std::cerr<<"Total Priciple Amount"<<total_principle_amount<<" Total Interrest Amount "<<total_interrest_amount<<std::endl;
        if(this->extra_amount!=0)
        {
            installment_amnt=ceil(installment_amnt);
            //installment_amnt=round(installment_amnt);
            //ins_installment_amnt=installment_amnt-my_round(pri_installment_amnt);
            pri_installment_amnt=loan_amount-total_principle_amount;
            ins_installment_amnt=installment_amnt-my_round(pri_installment_amnt);
        }
        //std::cerr<<"Installment Amnt"<<installment_amnt<<" Pri Installment Amnt "<<pri_installment_amnt<<" Ins Installment Amnt "<<ins_installment_amnt<<std::endl;

        this->first_repayment_date =schedule[0];
        this->last_repayment_date=d_date;

        //total_principle_amount=total_principle_amount+my_round(pri_installment_amnt);
        //total_interrest_amount=total_interrest_amount+my_round(ins_installment_amnt);
        
        total_principle_amount=total_principle_amount+pri_installment_amnt;
        total_interrest_amount=total_interrest_amount+ins_installment_amnt;
        
        real_principle_installment_amount=(pri_installment_amnt);
        real_interrest_installment_amount=(ins_installment_amnt);
        real_total_principle_amount=real_total_principle_amount+real_principle_installment_amount;
        real_total_interrest_amount=real_total_interrest_amount+real_interrest_installment_amount;


        schedule_data temp_schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt),real_principle_installment_amount,real_interrest_installment_amount);
        schedule.push_back(d_date);
        schedule_datum.push_back(temp_schedule_data);
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), temp_schedule_data));

        /*
        schedule.push_back(d_date);
        schedule_datum.push_back(schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt)));
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,my_round(installment_amnt),my_round(pri_installment_amnt),my_round(ins_installment_amnt))));
        */ 
        //std::cerr<<"Total Priciple Amount"<<total_principle_amount<<" Total Interrest Amount "<<total_interrest_amount<<std::endl;
        //std::cerr<<"Installment Amnt"<<installment_amnt<<" Pri Installment Amnt "<<pri_installment_amnt<<" Ins Installment Amnt "<<ins_installment_amnt<<std::endl;
        //std::cerr<<"Installment Amnt"<<my_round(installment_amnt)<<" Pri Installment Amnt "<<my_round(pri_installment_amnt)<<" Ins Installment Amnt "<<my_round(ins_installment_amnt)<<std::endl;
        ////exit(1);
        total_payable_amount=total_principle_amount+total_interrest_amount;
        //std::cerr<<"Total Payable "<<total_payable_amount<<std::endl;
        this->first_repayment_date =schedule[0];
        this->last_repayment_date=d_date;
        /*
        schedule.push_back(d_date);
        schedule_datum.push_back(schedule_data(d_date,installment_amnt,pri_installment_amnt,ins_installment_amnt));
        schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(d_date), schedule_data(d_date,installment_amnt,pri_installment_amnt,ins_installment_amnt)));
        */

    }
    updateFirstLastInstallmentNumber();
    //std::cerr<<"updateFirstLastInstallmentNumber Total Payable Amount"<<schedule_datum.size()<<" "<<total_payable_amount<<std::endl;
}


void loan::change_Schedule_at_index(int i, boost::gregorian::date d_date) {
    schedule[i] = d_date;
}

boost::gregorian::date loan::getSchedule_by_installment_no(int i) {

	boost::gregorian::date d;
	d=schedule[i];
	return d;
}



void loan::calculate_Instalment_amounts(int installment_no,double &installment_amnt,double &pri_installment_amnt){
			//if(boost::iequals(this->interest_calculation_method,"FLAT")){


				double total_amnt= (loan_amount+(loan_period_in_month/12)*(interest_rate/100)*loan_amount);
				installment_amnt=total_amnt/number_of_installment;
				pri_installment_amnt=loan_amount/number_of_installment;

				char buffer [50];
				sprintf(buffer,"%0.2f",installment_amnt);
				installment_amnt=strtod(buffer,NULL);
				//installment_amnt=atof(buffer);

				sprintf(buffer,"%0.2f",pri_installment_amnt);
				pri_installment_amnt=strtod(buffer,NULL);
				//pri_installment_amnt=atof(buffer);

				//std::cerr<<"\n"<<number_of_installment<<" "<<installment_no<<std::endl;
				if(installment_no==number_of_installment){

					installment_amnt=total_amnt-(installment_amnt*(number_of_installment-1));
					pri_installment_amnt=loan_amount-(pri_installment_amnt*(number_of_installment-1));

					sprintf(buffer,"%0.2f",installment_amnt);
					installment_amnt=strtod(buffer,NULL);
					//installment_amnt=atof(buffer);

					
					sprintf(buffer,"%0.2f",pri_installment_amnt);
					pri_installment_amnt=strtod(buffer,NULL);
					//pri_installment_amnt=atof(buffer);
				}

			//}
			/*
			else if(boost::iequals(this->interest_calculation_method,"DECLINING_METHOD")){

				double loan_period_in_month_d=loan_period_in_month;

				double total_amnt= (loan_amount+(loan_period_in_month_d/12)*(interest_rate/100)*loan_amount);
				installment_amnt=total_amnt/number_of_installment;
				pri_installment_amnt=loan_amount/number_of_installment;


			}
			*/

	}
        /*
	double loan::calculate_Instalment_amount(int installment_no){
		//if(boost::iequals(this->interest_calculation_method,"FLAT")){
			double loan_period_in_month_d=loan_period_in_month;
			double total_amnt= (loan_amount+((loan_period_in_month_d/12)*(interest_rate/100))*loan_amount);
			double ints_amount=total_amnt/number_of_installment;
			//ints_amount=rnd(ints_amount);
			//if(installment_no==number_of_installment){
				//return (ints_amount+(total_amnt-ints_amount*number_of_installment));
			//}
			//if(ints_amount){
				//std::cout<<installment_no<<" "<<loan_period_in_month_d<<" "<<total_amnt<<" "<<ints_amount<<std::endl;

			//}
			return ints_amount;
		//}
	}

	double loan::calculate_principle_Instalment_amount(int installment_no){
		//if(boost::iequals(this->interest_calculation_method,"FLAT")){
			double total_amnt= (loan_amount);
			double ints_amount=total_amnt/number_of_installment;
			//ints_amount=rnd(ints_amount);
			//if(installment_no==number_of_installment){
				//return (ints_amount+(total_amnt-ints_amount*number_of_installment));
			//}

			if(ints_amount){
				std::cout<<installment_no<<" "<<total_amnt<<" "<<ints_amount<<std::endl;

			}

			return ints_amount;
		//}
	}
        */
