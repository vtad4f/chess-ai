#pragma once

#include <exception>
#include <string>


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  This exception class is intended to be thrown with a cached
///           message in the event of an error
///
////////////////////////////////////////////////////////////////////////////////
class Error : public std::exception
{
public:
   Error(const std::string& errMsg);
   virtual const char* what() const noexcept override;
   
protected:
   std::string m_ErrMsg;
};


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Throw the error with the message
///
///   @param errMsg  The error message to throw
///
////////////////////////////////////////////////////////////////////////////////
#define EXIT(errMsg) ({                                  \
   throw Error(std::string(__func__) + ": " + (errMsg)); \
})


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  If the condition is false, throw the error with the message
///
///   @param condition  The condition to check
///
////////////////////////////////////////////////////////////////////////////////
#define ASSERT(condition) ({        \
   if (!(condition))                \
   {                                \
      EXIT("Expected " #condition); \
   }                                \
})


////////////////////////////////////////////////////////////////////////////////
///
///   @brief  Define additional asserts to check specific conditions
///
////////////////////////////////////////////////////////////////////////////////
#define ASSERT_EQ(a,b) ({ ASSERT((a) == (b)); })
#define ASSERT_NE(a,b) ({ ASSERT((a) != (b)); })
#define ASSERT_LT(a,b) ({ ASSERT((a) <  (b)); })
#define ASSERT_LE(a,b) ({ ASSERT((a) <= (b)); })
#define ASSERT_GT(a,b) ({ ASSERT((a) >  (b)); })
#define ASSERT_GE(a,b) ({ ASSERT((a) >= (b)); })
#define ASSERT_IN_RANGE(x,a,b) ({ ASSERT((a) <= (x) && (x) < (b)); })

