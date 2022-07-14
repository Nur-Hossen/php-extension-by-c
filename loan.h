/* 
 * File:   loan.h
 * Author: Murad
 *
 * Created on February 6, 2011, 4:57 PM
 */

#ifndef LOAN_H
#define	LOAN_H

#include <cmath>
#include <stdio.h>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/math/special_functions/round.hpp"
#include "boost/functional/hash.hpp"
#include <boost/algorithm/string.hpp>
#include "schedule_data.h"
#include "ext/hash_map"



namespace boost{namespace gregorian{
    inline std::size_t hash_value(const boost::gregorian::date& b)
    {
        boost::hash<long> hasher;
        return hasher(b.julian_day());
    }
}};

struct ltstr
{
  bool operator()(boost::gregorian::date d1, boost::gregorian::date d2) const
  {
	  
	  if(d1<d2){
		  return -1;
	  }
	  else if(d1>d2){
		  return 1;
	  }
          
        return 0;
  }
};

namespace __gnu_cxx {

    template<> struct hash< std::string > {

        size_t operator()(const std::string & x) const {
            return hash< const char* >()(x.c_str());
        }
    };
}
//typedef __gnu_cxx::hash_map<std::string, schedule_data> schedule_datum_hash_map;
    
class schedule_datum_hash_map{
    double loan_amount;
    double total_payable_amount;
    public:
        
    schedule_datum_hash_map(){
        loan_amount=0;
        total_payable_amount=0;    
    }
    void set_loan_amount(double loan_amount){
        this->loan_amount=loan_amount;
    }
    void set_total_payable_amount(double total_payable_amount){
        this->total_payable_amount=total_payable_amount;
    }
    __gnu_cxx::hash_map<std::string, schedule_data> sch_h_data;
    
    void clear(){
        sch_h_data.clear();
    }
    
    void insert(std::pair<std::string,schedule_data> data){
        
       sch_h_data.insert(data);
        
    }
};
class schedule_datas{
    double loan_amount;
    double total_payable_amount;
    
public:
    schedule_datas(){
        loan_amount=0;
        total_payable_amount=0;
    }
    
    void set_loan_amount(double loan_amount){
        this->loan_amount=loan_amount;
    }
    void set_total_payable_amount(double total_payable_amount){
        this->total_payable_amount=total_payable_amount;
    }
    std::vector<schedule_data> sch_data;
    void clear(){
        sch_data.clear();
    }
    size_t size(){
        return sch_data.size();
    }
    void push_back(schedule_data data){
        
        sch_data.push_back(data);
        
    }
    
    
    schedule_data operator[](size_t __n) { return  sch_data[__n];}
    //schedule_data operator[](size_t __n) { return *(this->_M_impl._M_start + __n); }
    /*
    size_t size(){
        return this->size();
    }
    
    void clear(){
        this->clear();
    }
    void push_back(const schedule_data &_X){
        this->push_back(_X);
    }
    */
    
};

class loan {
private:

    int id;
    std::string customized_loan_no;
    std::string loan_application_no;
    int branch_id;
    int samity_id;
    int product_id;
    int member_id;
    int purpose_id;
    int funding_org_id;
    std::string interest_calculation_method;
    std::string decline_period;
    double loan_amount;
    double installment_amountdb;    
    double interest_rate;
    double interest_amount;
    double discount_interest_amount;
    double insurance_guarantor_percentage;
    double insurance_guarantor_amount;
    double total_payable_amount;
    int number_of_installment;
    double installment_amount;
    std::string repayment_frequency;
    std::string grace_period;
    double loan_period_in_month;
    double loan_period_in_day;
    int is_loan_period_in_day_fixed;
    std::string mode_of_interest;
    int interest_rate_calculation_amount;
    int cycle;
    boost::gregorian::date first_repayment_date;
    boost::gregorian::date last_repayment_date;
    int authorization_status;
    int disburse_registration_no;
    std::string disbursed_by;
    boost::gregorian::date disburse_date;
    std::string authorized_by;
    boost::gregorian::date authorized_date;
    int is_authorized;
    int ledger_id;
    int voucher_status;
    int fully_paid_registration_no;
    int overdue_registration_no;
    boost::gregorian::date loan_fully_paid_date;
    int current_status;
    boost::gregorian::date loan_closing_date;
    int loan_closing_info_verified_by;
    int is_deleted;
    int is_loan_fully_paid;
    std::string loan_type;
    int skip_last_week_calculation_monthly;
    boost::gregorian::date *system_date;
    boost::gregorian::date prev_repayment_date;    
    
    //std::vector<int> transferred_installment_numbers;
    typedef __gnu_cxx::hash_map<int, boost::gregorian::date> samity_transfer_info;
    samity_transfer_info samity_transfers;
    
    

public:
    
    double interest_index;
    
    double total_amnt; //= (loan_amount+(loan_period_in_month/12)*(interest_rate/100)*loan_amount);
    double installment_amnt; //=total_amnt/number_of_installment;
    double pri_installment_amnt; //=loan_amount/number_of_installment;
    double ins_installment_amnt;
    double no_of_payment_per_year; // = (number_of_installment*12)/loan_period_in_month;
    double interest_per_installment; //=interest_rate/(no_of_payment_per_year*100);
    
    double loan_amnt;
    
    double extra_amount;
    
    double total_paid_amount;
    
    double total_installment_amount;
    double total_interrest_amount;
    double total_principle_amount;
    
    double real_principle_installment_amount;
    double real_interrest_installment_amount;
        
    double real_total_interrest_amount;
    double real_total_principle_amount;
    
    int actual_number_of_installments;

    std::vector<boost::gregorian::date> schedule;

    //std::vector<schedule_data> schedule_datum;
    schedule_datas schedule_datum;

    //typedef __gnu_cxx::hash_map<std::string, schedule_data> schedule_datum_hash_map;
    schedule_datum_hash_map schedule_datum_hash;
    
    int is_transferred_in;
    boost::gregorian::date transfer_in_date;
    int is_transferred_in_flag;
    int is_transferred_out;
    boost::gregorian::date transfer_out_date;
    int is_transferred_out_flag;
    int is_transferred;
    
    int first_installment_number;
    int last_installment_number;
    int original_loan_id;
    
    typedef __gnu_cxx::hash_map<std::string, unsigned int> week_day_hash;
    week_day_hash week_days;
    
    double one_time_weekly_priciple_paid_amount;
    double one_time_weekly_priciple_paid_amount_murad;
    boost::gregorian::date prev_schedule_date_murad;
    boost::gregorian::date prev_schedule_date_murad_temp;
    boost::gregorian::date schedule_date_murad;
    __gnu_cxx::hash_map<boost::gregorian::date, double, boost::hash<boost::gregorian::date> > one_time_weekly_loans_transactions__info;
    __gnu_cxx::hash_map<boost::gregorian::date, std::string, boost::hash<boost::gregorian::date> > loan_transfer_daychange__info;
    __gnu_cxx::hash_map<boost::gregorian::date, int, boost::hash<boost::gregorian::date> > loan_transfer_samitychange__info;
    boost::gregorian::greg_weekday GetLoan_week_day(boost::gregorian::date d_date){
        std::string day;
        int diff=10000;
        for(__gnu_cxx::hash_map<boost::gregorian::date, std::string, boost::hash<boost::gregorian::date> >::iterator it = loan_transfer_daychange__info.begin();it != loan_transfer_daychange__info.end();it++){
            if(d_date>=it->first && d_date.julian_day()-it->first.julian_day()<=diff && d_date.julian_day()-it->first.julian_day()>=0){
                day=it->second;
                diff=d_date.julian_day()-it->first.julian_day();
            }
        }
         return boost::gregorian::greg_weekday(week_days[day]);
    }
    
    int GetSamity_id_consider_transfer(boost::gregorian::date d_date){
        int samity=this->samity_id;
        int diff=401767;
        if(this->is_transferred!=0){
            for(__gnu_cxx::hash_map<boost::gregorian::date, int, boost::hash<boost::gregorian::date> >::iterator it = loan_transfer_samitychange__info.begin();it != loan_transfer_samitychange__info.end();it++){
                if(d_date>=it->first && d_date.julian_day()-it->first.julian_day()<=diff && d_date.julian_day()-it->first.julian_day()>=0){
                    samity=it->second;
                    //std::cerr<<"samity_id "<<samity<<std::endl;
                    diff=d_date.julian_day()-it->first.julian_day();
                }
            }
        }
        //std::cerr<<"Return samity_id "<<samity<<std::endl;
        return samity;
    }
    
    bool is_transferred_out_before_date(boost::gregorian::date d_date){
    
        if(this->is_transferred_out==false)
            return false;
        if(this->transfer_out_date>d_date)
            return false;
        
        return true;
        
    }
    
    double penalty;
    double waiver;
    double rebate;
    
    boost::gregorian::date rebate_date;
    
    int getOriginal_loan_id(){
        return original_loan_id;
    }
    
    
    int getFirstInstallmentNumber(){
        return first_installment_number;
    }
    int getLastInstallmentNumber(){
        return last_installment_number;
    }
    
    void updateFirstLastInstallmentNumber(){
        
        first_installment_number=0;
        last_installment_number=static_cast<int>(this->schedule_datum.size());
        
        
        if(this->is_transferred){
            for(int i=0;i<this->schedule_datum.size();i++){
                if(this->is_transferred_in_flag!=0){
                    if(this->schedule_datum[i].schedule_date>=this->transfer_in_date){
                        first_installment_number=i;
                        this->is_transferred_in_flag=0;
                    }
                }
                if(this->is_transferred_out_flag!=0){
                    if(this->schedule_datum[i].schedule_date>=this->transfer_out_date){
                        last_installment_number=i;
                        this->is_transferred_out_flag=0;
                        break;
                    }
                }
            }
            first_installment_number=0;
            last_installment_number=this->schedule_datum.size();
        }
        //this->apply_rebate();
        
    }
    
    void apply_rebate(){
        //std::cerr<<"rebate "<<rebate<<" system_date "<<(*this->system_date)<<" rebate_date "<<this->rebate_date<<std::endl;
        if(this->rebate>0 && (*this->system_date)>=this->rebate_date){
            schedule_datas temp_schedule_datum;
            schedule_datum_hash_map temp_schedule_datum_hash;
            
            double local_rebate=rebate;
            double local_installment_amount=0;
            double local_principle_installment_amount=0;
            double local_interrest_installment_amount=0;
            
            double local_real_principle_installment_amount=0;
            double local_real_interrest_installment_amount=0;
            
            boost::gregorian::date local_d_date;
            for(int i=0;i<this->schedule_datum.size();i++){
                //std::cerr<<"local_rebate "<<local_rebate;
                //std::cerr<<" schedule_date "<<this->schedule_datum[i].schedule_date<<" installment_amount "<<this->schedule_datum[i].installment_amount<<" interrest_installment_amount "<<this->schedule_datum[i].interrest_installment_amount<<std::endl;
                if(this->schedule_datum[i].schedule_date<=this->rebate_date || local_rebate==0){
                    temp_schedule_datum.push_back(this->schedule_datum[i]);
                    temp_schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(local_d_date),this->schedule_datum[i]));
                    continue;
                }
                if(local_rebate>=this->schedule_datum[i].interrest_installment_amount){
                    
                    //std::cerr<<"local_rebate "<<local_rebate;
                    //std::cerr<<" schedule_date "<<this->schedule_datum[i].schedule_date<<" installment_amount "<<this->schedule_datum[i].installment_amount<<" interrest_installment_amount "<<this->schedule_datum[i].interrest_installment_amount<<std::endl;
                
                    this->total_payable_amount-=this->schedule_datum[i].interrest_installment_amount;
                    this->total_interrest_amount-=this->schedule_datum[i].interrest_installment_amount;
                    local_rebate-=this->schedule_datum[i].interrest_installment_amount;
                    local_d_date=this->schedule_datum[i].schedule_date;
                    local_installment_amount=this->schedule_datum[i].installment_amount-this->schedule_datum[i].interrest_installment_amount;
                    local_principle_installment_amount=this->schedule_datum[i].principle_installment_amount;
                    local_interrest_installment_amount=0;
                    
                    local_real_principle_installment_amount=this->schedule_datum[i].real_principle_installment_amount;
                    local_real_interrest_installment_amount=0;
                    
                    temp_schedule_datum.push_back(schedule_data(local_d_date,local_installment_amount,local_principle_installment_amount,local_interrest_installment_amount,local_real_principle_installment_amount,local_real_interrest_installment_amount));
                    temp_schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(local_d_date), schedule_data(local_d_date,local_installment_amount,local_principle_installment_amount,local_interrest_installment_amount,local_real_principle_installment_amount,local_real_interrest_installment_amount)));
                    //std::cerr<<"local_rebate "<<local_rebate;
                    //std::cerr<<" schedule_date "<<this->schedule_datum[i].schedule_date<<" installment_amount "<<this->schedule_datum[i].installment_amount<<" interrest_installment_amount "<<this->schedule_datum[i].interrest_installment_amount<<std::endl;
                
                }
                else{
                    this->total_payable_amount-=local_rebate;
                    this->total_interrest_amount-=local_rebate;
                    local_d_date=this->schedule_datum[i].schedule_date;
                    local_installment_amount=this->schedule_datum[i].installment_amount-local_rebate;
                    
                    local_principle_installment_amount=this->schedule_datum[i].principle_installment_amount;
                    local_interrest_installment_amount=this->schedule_datum[i].interrest_installment_amount-local_rebate;;
                    
                    local_real_principle_installment_amount=this->schedule_datum[i].real_principle_installment_amount;
                    local_real_interrest_installment_amount=this->schedule_datum[i].real_interrest_installment_amount-local_rebate;;
                    
                    
                    this->schedule_datum[i]=schedule_data(local_d_date,local_installment_amount,local_principle_installment_amount,local_interrest_installment_amount,local_real_principle_installment_amount,local_real_interrest_installment_amount);
                    
                    temp_schedule_datum.push_back(schedule_data(local_d_date,local_installment_amount,local_principle_installment_amount,local_interrest_installment_amount,local_real_principle_installment_amount,local_real_interrest_installment_amount));
                    temp_schedule_datum_hash.insert(std::make_pair(boost::gregorian::to_iso_extended_string(local_d_date), schedule_data(local_d_date,local_installment_amount,local_principle_installment_amount,local_interrest_installment_amount,local_real_principle_installment_amount,local_real_interrest_installment_amount)));
                    
                    local_rebate=0;
                    //break;
                }
                
                
            }
            this->schedule_datum=temp_schedule_datum;
            this->schedule_datum_hash=temp_schedule_datum_hash;
        }
        ////exit(1);
    }




    loan(
            __gnu_cxx::hash_map<std::string, double> one_time_weekly_loans_transactions__info,
            __gnu_cxx::hash_map<std::string, int> loan_transfer_samitychange__info,
            __gnu_cxx::hash_map<std::string, std::string> loan_transfer_daychange__info,
            int id,
            int branch_id,
            int samity_id,
            int product_id,
            int purpose_id,
            int member_id,
            double loan_amount,
            double interest_rate,
            int number_of_installment,
            std::string repayment_frequency,
            std::string grace_period,
            double loan_period_in_month,
            boost::gregorian::date first_repayment_date,
            boost::gregorian::date last_repayment_date,
            boost::gregorian::date disburse_date,
            boost::gregorian::date loan_fully_paid_date,
            int current_status,
            int is_loan_fully_paid,
            int is_authorized,
            std::string interest_calculation_method,
            std::string mode_of_interest,
            boost::gregorian::date loan_closing_date,
            double extra_amount,
            int original_loan_id,
            int is_transferred_in,
            std::string transfer_in_date,
            int is_transferred_out,
            std::string transfer_out_date,
            double penalty,
            double rebate,
            boost::gregorian::date rebate_date,
            double waiver,
            std::string loan_type,
            boost::gregorian::date *system_date,
            std::string interest_payment_frequency,
            std::string repayment_mode_for_monthly_loan,
            std::string repayment_day_or_week,
            double interest_index,
            int enforce_number_of_installment,
            int enforced_install_number,
            std::string decline_period,
            int is_show_rec_after_loan_period,
            double installment_amountdb,
            double loan_period_in_day,
            int is_loan_period_in_day_fixed,
            int skip_last_week_calculation_monthly,
            std::string service_charge_taken_time,
            int cycle
            );

    int enforce_number_of_installment;
    int is_show_rec_after_loan_period;
    int enforced_install_number;
    std::string interest_payment_frequency;
    std::string repayment_mode_for_monthly_loan;
    std::string service_charge_taken_time;
    std::string repayment_day_or_week;
    
    std::string GetInterest_payment_frequency() const{
        return interest_payment_frequency;
    }
    std::string GetRepayment_mode_for_monthly_loan() const{
        return repayment_mode_for_monthly_loan;
    }  
    std::string GetService_charge_taken_time() const{
        return service_charge_taken_time;
    }
    std::string GetRepayment_day_or_week() const{
        return repayment_day_or_week;
    }
    
    int GetRepayment_day() const{
        //std::cerr<<"GetRepayment_day "<<repayment_day_or_week<<std::endl;
        int repayment_day=0;
        if(repayment_day_or_week.find("W")==std::string::npos)
            repayment_day= atoi(repayment_day_or_week.c_str());
        //std::cerr<<"repayment_day "<<repayment_day<<std::endl;
        return repayment_day;
    }
    
    int GetRepayment_week() const{
        //std::cerr<<"GetRepayment_week "<<repayment_day_or_week<<std::endl;
        int repayment_week=0;
        if(repayment_day_or_week.find("W")!=std::string::npos){
            repayment_week=atoi(repayment_day_or_week.substr(0,repayment_day_or_week.find("W")).c_str());
        }
        //std::cerr<<"repayment_week "<<repayment_week<<std::endl;
        return repayment_week;
    }
    
    std::string GetLoan_type() const{
        return loan_type;
    }
    int GetAuthorization_status() const;

    void SetAuthorization_status(int authorization_status);

    int GetIs_authorized() const;

    void SetIs_authorized(int is_authorized);

    std::string GetAuthorized_by() const;

    void SetAuthorized_by(std::string authorized_by);

    boost::gregorian::date GetAuthorized_date() const;

    void SetAuthorized_date(boost::gregorian::date authorized_date);

    int GetBranch_id() const;

    void SetBranch_id(int branch_id);

    int GetCurrent_status() const;

    void SetCurrent_status(int current_status);

    std::string GetCustomized_loan_no() const;

    void SetCustomized_loan_no(std::string customized_loan_no);

    int GetCycle() const;

    void SetCycle(int cycle);

    boost::gregorian::date GetDisburse_date() const;

    void SetDisburse_date(boost::gregorian::date disburse_date);
    
    boost::gregorian::date GetTransfer_in_date() const;

    void SetTransfer_in_date(boost::gregorian::date transfer_in_date);
    
    boost::gregorian::date GetTransfer_out_date() const;

    void SetTransfer_out_date(boost::gregorian::date transfer_out_date);

    int GetDisburse_registration_no() const;

    void SetDisburse_registration_no(int disburse_registration_no);

    std::string GetDisbursed_by() const;

    void SetDisbursed_by(std::string disbursed_by);

    double GetDiscount_interest_amount() const;

    void SetDiscount_interest_amount(double discount_interest_amount);

    boost::gregorian::date GetFirst_repayment_date(bool actual=true) const;

    void SetFirst_repayment_date(boost::gregorian::date first_repayment_date);

    int GetFully_paid_registration_no() const;

    void SetFully_paid_registration_no(int fully_paid_registration_no);

    int GetFunding_org_id() const;

    void SetFunding_org_id(int funding_org_id);

    int GetId() const;

    void SetId(int id);

    double GetInstallment_amount() const;

    void SetInstallment_amount();

    double GetInsurance_guarantor_amount() const;

    void SetInsurance_guarantor_amount(double insurance_guarantor_amount);

    double GetInsurance_guarantor_percentage() const;

    void SetInsurance_guarantor_percentage(double insurance_guarantor_percentage);

    double GetInterest_amount() const;

    void SetInterest_amount(double interest_amount);

    std::string GetInterest_calculation_method() const;

    void SetInterest_calculation_method(std::string interest_calculation_method);

    std::string GetDecline_Period() const;

    void SetDecline_Period(std::string decline_period);
    
    double GetInterest_rate() const;

    void SetInterest_rate(double interest_rate);

    int GetInterest_rate_calculation_amount() const;

    void SetInterest_rate_calculation_amount(int interest_rate_calculation_amount);

    int GetIs_deleted() const;

    void SetIs_deleted(int is_deleted);

    boost::gregorian::date GetLast_repayment_date() const;

    void SetLast_repayment_date(boost::gregorian::date last_repayment_date);

    int GetLedger_id() const;

    void SetLedger_id(int ledger_id);

    double GetLoan_amount() const;

    void SetLoan_amount(double loan_amount);
    
    double GetInstallment_Amount_db() const;

    void SetInstallment_Amount_db(double installment_amountdb);

    std::string GetLoan_application_no() const;

    void SetLoan_application_no(std::string loan_application_no);

    boost::gregorian::date GetLoan_closing_date() const;

    void SetLoan_closing_date(boost::gregorian::date loan_closing_date);

    int GetLoan_closing_info_verified_by() const;

    void SetLoan_closing_info_verified_by(int loan_closing_info_verified_by);

    boost::gregorian::date GetLoan_fully_paid_date() const;

    void SetLoan_fully_paid_date(boost::gregorian::date loan_fully_paid_date);

    double GetLoan_period_in_month() const;

    void SetLoan_period_in_month(double loan_period_in_month);
    
    double GetLoan_period_in_day() const;

    void SetLoan_period_in_day(double loan_period_in_day);

    int GetMember_id() const;

    void SetMember_id(int member_id);

    std::string GetMode_of_interest() const;

    void SetMode_of_interest(std::string mode_of_interest);

    int GetNumber_of_installment() const;
    
    int GetActualNumber_of_installment() const;

    void SetNumber_of_installment(int number_of_installment);

    int GetOverdue_registration_no() const;

    void SetOverdue_registration_no(int overdue_registration_no);

    int GetProduct_id() const;

    void SetProduct_id(int product_id);

    int GetPurpose_id() const;

    void SetPurpose_id(int purpose_id);

    std::string GetRepayment_frequency() const;

    void SetRepayment_frequency(std::string repayment_frequency);

    std::string GetGrace_period() const;

    void SetGrace_period(std::string grace_period);
    
    int GetSamity_id() const;

    void SetSamity_id(int samity_id);

    double GetTotal_payable_amount() const;

    void SetTotal_payable_amount();

    int GetVoucher_status() const;

    void SetVoucher_status(int voucher_status);

    void SetIs_loan_fully_paid(int is_loan_fully_paid);

    int GetIs_loan_fully_paid() const;

    
    
    void addNewSchedule_date(boost::gregorian::date d_date);
    
    void addNewOneTimeSchedule_date(boost::gregorian::date& d_date);
    void addNewOneTimeSchedule_date_multiple(boost::gregorian::date& d_date);

    void addNewFLATSchedule_date(boost::gregorian::date& d_date);
    
    void addNewOneTimeFLATSchedule_date(boost::gregorian::date& d_date, bool is_round=true);
    void addNewOneTimeFLATSchedule_dateMonthly(boost::gregorian::date& d_date, bool is_round=true);
    
    void addLastOneTimeFLATSchedule_date(boost::gregorian::date& d_date);
    void addLastOneTimeFLATSchedule_dateMonthly(boost::gregorian::date& d_date);
    
    void addNewMonthlyPricipleFirstSchedule_date(boost::gregorian::date& d_date);
    
    void addNewMonthlyPricipleLastSchedule_date(boost::gregorian::date& d_date);

    void addNewDECLININGSchedule_date(boost::gregorian::date& d_date);
    void addNewDECLININGSchedule_date_daily_basis(boost::gregorian::date& d_date);
    
    void addLastFLATSchedule_date(boost::gregorian::date& d_date);
    void addLastFLATSchedule_date_enforce(boost::gregorian::date& d_date,boost::gregorian::date& d_date_first,int enforce_number);   
    
    void addLastDECLININGSchedule_date(boost::gregorian::date& d_date);
    
    void addNewDFDSchedule_date(boost::gregorian::date& d_date);
    void addNewDFDOneTimeSchedule_dateDailyBasis(boost::gregorian::date& d_date);
    
    void addLastDFDSchedule_date_enforce(boost::gregorian::date& d_date,boost::gregorian::date& d_date_first,int enforce_number);   
    
    void addLastDFDSchedule_date(boost::gregorian::date& d_date);
    
    void addNewASMSchedule_date(boost::gregorian::date& d_date);
    
    void addLastASMSchedule_date(boost::gregorian::date& d_date);
    
    void addNewEFRRAPSchedule_date(boost::gregorian::date& d_date);
    void addNewEFRRAPSchedule_date_new(boost::gregorian::date& d_date);
    
    void addLastEFRRAPSchedule_date(boost::gregorian::date& d_date);
    void addLastEFRRAPSchedule_date_new(boost::gregorian::date& d_date);
    
    void addNewSCTBISchedule_dateDFD(boost::gregorian::date& d_date);
    void addLastSCTBISchedule_dateDFD(boost::gregorian::date& d_date);
    void Set_skip_last_week_calculation_monthly(int skip_last_week_calculation_monthly);
    int Get_skip_last_week_calculation_monthly();
    double rnd(double n) {
        n = 125.987650;
        double t;
        double i;
        //t=modf(n,i);
        t = n - floor(n);
        char buffer [50];
        sprintf(buffer, "%0.2f", n);
        n = atof(buffer);
        //std::cout<<n<<" "<<t<<std::endl;
        return n;

        //char buffer [50];
        //int n, a=5, b=3;
        //n=sprintf (buffer, "%d plus %d is %d", a, b, a+b);

        if (t >= 0.5) {
            n = n * 100; //where n is the multi-decimal float
            printf("n:%f\n", n);
            int m = ceil(n);
            printf("m:%d n:%f\n", m, n);
            n = (m * 0.01);
            printf("m:%d n:%f\n", m, n);
        } else {
            n *= 100; //where n is the multi-decimal float
            n = floor(n);
            n /= 100;
        }
        printf("%f %f\n", n, t);
        //exit(1);
        return n;
    }

    double atof(std::string input) {

    }

    void calculate_Instalment_amounts(int installment_no, double &installment_amnt, double &pri_installment_amnt);
    double calculate_Instalment_amount(int installment_no);

    double calculate_principle_Instalment_amount(int installment_no);

    void change_Schedule_at_index(int i, boost::gregorian::date d_date);

    boost::gregorian::date getSchedule_by_installment_no(int i);

    loan();
    loan(const loan& orig);
    ~loan();

};

#endif	/* LOAN_H */
