#pragma once
#include "Serializable.hpp"
#include "EntityObject.hpp"

class Date : public Serializable, public EntityObject {  
private:  
   time_t _date = 0;  
   const std::string _format = "%Y-%m-%d";  
public:  
   Date();
   Date(time_t date);
   Date(const Date& other);
   ~Date();   

   std::string formatDate() const;  
   void parseDate(const std::string& dateStr);  

   time_t getDate() const;  

   std::string toString() const override;  
   std::string serialize() const override;  

   static time_t getInputDate(const std::string& prompt);

   Date& operator=(const Date& other);
};