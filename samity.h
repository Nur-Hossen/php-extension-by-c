/* 
 * File:   samity.h
 * Author: amlan
 *
 * Created on February 6, 2011, 4:57 PM
 */

#ifndef SAMITY_H
#define	SAMITY_H

#include"boost/date_time/gregorian/gregorian.hpp"
#include "boost/functional/hash.hpp"
#include "ext/hash_map"
#include "loan.h"
#include <fstream>

//#include "schedulerSingleton.h"
/*
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
*/
class samity {
private:

    int id;
    int branch_id;
    int working_area_id;
    int field_officer_id;
    int product_id;
    std::string name;
    std::string code;
    std::string registration_no;
    std::string samity_type;
    boost::gregorian::date opening_date;
    int created_by;
    boost::gregorian::date created_date;
    int updated_by;
    boost::gregorian::date updated_date;
    int is_deleted;
    int deleted_by;
    boost::gregorian::date delete_date;
    int status;

    //typedef std::map<boost::gregorian::date, std::string,ltstr> samity_days_type; //act_date,day
 public:   
    typedef __gnu_cxx::hash_map<boost::gregorian::date, std::string,boost::hash<boost::gregorian::date> > samity_days_type;
    samity_days_type samity_days;
    
    typedef __gnu_cxx::hash_map<boost::gregorian::date, int,boost::hash<boost::gregorian::date> > samity_schedule_type;
    samity_schedule_type *samity_schedules;
    
    typedef __gnu_cxx::hash_map<int,boost::gregorian::date> samity_schedule_index_type;
    samity_schedule_index_type *samity_schedules_index;
    
    std::vector<boost::gregorian::date> samity_days_vector;
    
    std::string samity_day;
	
	std::string sam_day;


    boost::gregorian::date curr_act_date;
    
    
   



    samity_days_type::iterator it;
    bool samity_day_change;
    bool is_samity_day_obsolete;
	typedef __gnu_cxx::hash_map<std::string, unsigned int> week_day_hash;
	week_day_hash week_days;
        
         __gnu_cxx::hash_map<boost::gregorian::date, boost::gregorian::date,boost::hash<boost::gregorian::date> > *b_holiday;
    __gnu_cxx::hash_map<boost::gregorian::date, boost::gregorian::date,boost::hash<boost::gregorian::date> > *s_holiday;

//typedef __gnu_cxx::hash_map<std::string, std::vector<boost::gregorian::date> > schedule_dates;

    samity(
            int,
            int,
            int,
            int,
            int,
            std::string,
            std::string,
            std::string,
	    std::string,
            std::string,
            boost::gregorian::date,
            int,
            boost::gregorian::date,
            int,
            boost::gregorian::date,
            int,
            int,
            boost::gregorian::date,
            int,bool);

    int GetBranch_id() const;

    void SetBranch_id(int branch_id);

    std::string GetCode() const;

    void SetCode(std::string code);

    int GetCreated_by() const;

    void SetCreated_by(int created_by);

    boost::gregorian::date GetCreated_date() const;

    void SetCreated_date(boost::gregorian::date created_date);

    boost::gregorian::date GetDelete_date() const;

    void SetDelete_date(boost::gregorian::date delete_date);

    int GetDeleted_by() const;

    void SetDeleted_by(int deleted_by);

    int GetField_officer_id() const;

    void SetField_officer_id(int field_officer_id);

    int GetId() const;

    void SetId(int id);

    int GetIs_deleted() const;

    void SetIs_deleted(int is_deleted);

    std::string GetName() const;

    void SetName(std::string name);

    boost::gregorian::date GetOpening_date() const;

    void SetOpening_date(boost::gregorian::date opening_date);

    int GetProduct_id() const;

    void SetProduct_id(int product_id);

    std::string GetRegistration_no() const;

    void SetRegistration_no(std::string registration_no);

    std::string GetSamity_type() const;

    void SetSamity_type(std::string samity_type);

    int GetStatus() const;

    void SetStatus(int status);

    int GetUpdated_by() const;

    void SetUpdated_by(int updated_by);

    boost::gregorian::date GetUpdated_date() const;

    void SetUpdated_date(boost::gregorian::date updated_date);

    int GetWorking_area_id() const;

    void SetWorking_area_id(int working_area_id);

    boost::gregorian::greg_weekday GetSamity_week_day_local(boost::gregorian::date);
    boost::gregorian::greg_weekday GetSamity_week_day(boost::gregorian::date);
    
    bool GetIs_samity_day_obsolete() const;
    void SetIs_samity_day_obsolete(bool is_samity_day_obsolete);

    void add_samity_days(std::string day, std::string act_date = "1900-01-01");
    void set_samity_days(samity_days_type samity_days){
    	this->samity_days=samity_days;
    }

    std::string get_samity_day(boost::gregorian::date d_date);
    samity_days_type get_samity_days() const{
    	return this->samity_days;
    }
    
    void set_samity_day(std::string sam);
    
    boost::gregorian::date next_weekday(boost::gregorian::date d_date){
        /*
        if(s_holiday)
           while (s_holiday->find(d_date) != s_holiday->end()) {
     
                d_date = d_date + boost::gregorian::weeks(1);
                //d_date=boost::gregorian::next_weekday(d_date, this->GetSamity_week_day_local(d_date)); 
            }
            while (b_holiday->find(d_date) != b_holiday->end()) {
     
                d_date = d_date + boost::gregorian::weeks(1);
                //d_date=boost::gregorian::next_weekday(d_date, this->GetSamity_week_day_local(d_date)); 
            }
        */
        d_date=boost::gregorian::next_weekday(d_date, this->GetSamity_week_day_local(d_date));
        
        return d_date;
        
    }
    
    void make_samity_schedule(boost::gregorian::date start, boost::gregorian::date end,
    __gnu_cxx::hash_map<boost::gregorian::date, boost::gregorian::date,boost::hash<boost::gregorian::date> > *b_holiday
    ){
        //end=end+boost::gregorian::years(20);
        //boost::gregorian::day_iterator day_itr(start,1);
        //std::cerr<<start<<" "<<end<<std::endl;
        ////exit(1);
        this->b_holiday=b_holiday;
        boost::gregorian::date d_date=boost::gregorian::next_weekday(start, this->GetSamity_week_day_local(start));
        if(samity_days_vector[0]==d_date)
            d_date=boost::gregorian::next_weekday(start, this->GetSamity_week_day_local(d_date+boost::gregorian::days(1)));
        int count=0;
        int vector_count=0;
        //std::cerr<<d_date<<" ";
        for(;d_date<end;d_date=d_date+boost::gregorian::weeks(1)){
            //std::cerr<<d_date<<"\n";           
            
            if(samity_days_vector[vector_count]<=d_date){
               
                int pre=this->GetSamity_week_day_local(samity_days_vector[vector_count]-boost::gregorian::days(1)).as_number();
                pre=(pre+1)%7;
                int cur=this->GetSamity_week_day_local(d_date).as_number();
                cur=(cur+1)%7;
                
                if(cur-pre>=0){     
                    d_date=boost::gregorian::next_weekday(d_date, this->GetSamity_week_day_local(samity_days_vector[vector_count]));
                }
                else{
                    d_date=boost::gregorian::previous_weekday(d_date, this->GetSamity_week_day_local(samity_days_vector[vector_count]));
                }
                d_date = d_date - boost::gregorian::weeks(1);
                vector_count++;
                continue;
            }
            
            while (b_holiday->find(d_date) != b_holiday->end()) {
     
                 
                d_date = d_date + boost::gregorian::weeks(1);
                if(samity_days_vector[vector_count]<=d_date){

                    int pre=this->GetSamity_week_day_local(samity_days_vector[vector_count]-boost::gregorian::days(1)).as_number();
                    pre=(pre+1)%7;
                    int cur=this->GetSamity_week_day_local(d_date).as_number();
                    cur=(cur+1)%7;

                    if(cur-pre>=0){     
                        d_date=boost::gregorian::next_weekday(d_date, this->GetSamity_week_day_local(samity_days_vector[vector_count]));
                    }
                    else{
                        d_date=boost::gregorian::previous_weekday(d_date, this->GetSamity_week_day_local(samity_days_vector[vector_count]));
                    }
                    //d_date = d_date - boost::gregorian::weeks(1);
                    vector_count++;
                    continue;
                }
                //d_date = d_date + boost::gregorian::days(1);
                //d_date=boost::gregorian::next_weekday(d_date, this->GetSamity_week_day_local(d_date)); 
            }
            
            (*samity_schedules)[d_date]=count;
            (*samity_schedules_index)[count]=d_date;
            count++;
            
        }
        
        //this->print_samity_schedule();
       
    }
    void make_samity_schedule(boost::gregorian::date start, boost::gregorian::date end,
    __gnu_cxx::hash_map<boost::gregorian::date, boost::gregorian::date,boost::hash<boost::gregorian::date> > *b_holiday,
    __gnu_cxx::hash_map<boost::gregorian::date, boost::gregorian::date,boost::hash<boost::gregorian::date> > *s_holiday
    ){
        //end=end+boost::gregorian::years(20);
        //boost::gregorian::day_iterator day_itr(start,1);
        //std::cerr<<start<<" "<<end<<std::endl;
        ////exit(1);
        this->b_holiday=b_holiday;
        this->s_holiday=s_holiday;
        boost::gregorian::date d_date=boost::gregorian::next_weekday(start, this->GetSamity_week_day_local(start));
        if(samity_days_vector[0]==d_date)
            d_date=boost::gregorian::next_weekday(start, this->GetSamity_week_day_local(d_date+boost::gregorian::days(1)));
        int count=0;
        int vector_count=0;
        //std::cerr<<d_date<<" ";
        for(;d_date<end;d_date=d_date+boost::gregorian::weeks(1)){
           //std::cerr<<d_date<<"\n";           
            
           if(samity_days_vector[vector_count]<=d_date){
               
                int pre=this->GetSamity_week_day_local(samity_days_vector[vector_count]-boost::gregorian::days(1)).as_number();
                pre=(pre+1)%7;
                int cur=this->GetSamity_week_day_local(d_date).as_number();
                cur=(cur+1)%7;
                
                if(cur-pre>=0){     
                    d_date=boost::gregorian::next_weekday(d_date, this->GetSamity_week_day_local(samity_days_vector[vector_count]));
                }
                else{
                    d_date=boost::gregorian::previous_weekday(d_date, this->GetSamity_week_day_local(samity_days_vector[vector_count]));
                }
                d_date = d_date - boost::gregorian::weeks(1);
                vector_count++;
                continue;
            }
            /*
            while (s_holiday->find(d_date) != s_holiday->end()) {
                    
                d_date = d_date + boost::gregorian::weeks(1);
                if(samity_days_vector[vector_count]<=d_date){

                    int pre=this->GetSamity_week_day_local(samity_days_vector[vector_count]-boost::gregorian::days(1)).as_number();
                    pre=(pre+1)%7;
                    int cur=this->GetSamity_week_day_local(d_date).as_number();
                    cur=(cur+1)%7;

                    if(cur-pre>=0){     
                        d_date=boost::gregorian::next_weekday(d_date, this->GetSamity_week_day_local(samity_days_vector[vector_count]));
                    }
                    else{
                        d_date=boost::gregorian::previous_weekday(d_date, this->GetSamity_week_day_local(samity_days_vector[vector_count]));
                    }
                    //d_date = d_date - boost::gregorian::weeks(1);
                    vector_count++;
                    continue;
                }
                //d_date = d_date + boost::gregorian::days(1);
                //d_date=boost::gregorian::next_weekday(d_date, this->GetSamity_week_day_local(d_date)); 
            }
            */
            while ((b_holiday->find(d_date) != b_holiday->end()) || (s_holiday->find(d_date) != s_holiday->end()) ) {
     
                d_date = d_date + boost::gregorian::weeks(1);
                if(samity_days_vector[vector_count]<=d_date){

                    int pre=this->GetSamity_week_day_local(samity_days_vector[vector_count]-boost::gregorian::days(1)).as_number();
                    pre=(pre+1)%7;
                    int cur=this->GetSamity_week_day_local(d_date).as_number();
                    cur=(cur+1)%7;

                    if(cur-pre>=0){     
                        d_date=boost::gregorian::next_weekday(d_date, this->GetSamity_week_day_local(samity_days_vector[vector_count]));
                    }
                    else{
                        d_date=boost::gregorian::previous_weekday(d_date, this->GetSamity_week_day_local(samity_days_vector[vector_count]));
                    }
                    //d_date = d_date - boost::gregorian::weeks(1);
                    vector_count++;
                    continue;
                }
                //d_date = d_date + boost::gregorian::days(1);
                //d_date=boost::gregorian::next_weekday(d_date, this->GetSamity_week_day_local(d_date)); 
            }
            
            (*samity_schedules)[d_date]=count;
            (*samity_schedules_index)[count]=d_date;
            count++;
            
        }
        //this->print_samity_schedule();
        
    }
    
    void print_samity_schedule(){
        int count=0;
        for(samity_schedule_type::iterator it= this->samity_schedules->begin();it!=this->samity_schedules->end();++it){
            //std::cerr<<it->first<<" - "<<it->second<<" - "<<boost::gregorian::greg_weekday(it->first.day_of_week())<<std::endl;
            count++;
        }
        
        for(int it=0;it<this->samity_schedules->size();++it){
            //std::cerr<<it<<" - "<<(*samity_schedules_index)[it]<<" - "<<boost::gregorian::greg_weekday((*samity_schedules_index)[it].day_of_week())<<std::endl;
            
            count++;
        }
        //std::cerr<<id<<" "<<count<<std::endl;
         exit(1);
    }

    //std::string get_samity_day() const;
    
    samity();
    samity(const samity& orig);
    virtual ~samity();
private:

};

#endif	/* SAMITY_H */

