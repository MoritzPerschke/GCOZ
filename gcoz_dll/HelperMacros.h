
/*
  To precompile correctly I had to add /experimental:preprocessor to the cmd options of the solution.
  Otherwise, MSVC treats __VA_ARGS__ as one token and not as multiple seperated by a comma :))
  https://stackoverflow.com/questions/9183993/msvc-variadic-macro-expansion
*/

// https://stackoverflow.com/questions/45756920/c-c-preprocessor-extract-every-second-variadic-parameter
#define EVERY_SECOND0(...)

#define EVERY_SECOND1_(second, ...) second
#define EVERY_SECOND1(first, ...) EVERY_SECOND1_(__VA_ARGS__)

#define EVERY_SECOND2_(second, ...) second, EVERY_SECOND1(__VA_ARGS__)
#define EVERY_SECOND2(first, ...) EVERY_SECOND2_(__VA_ARGS__)

#define EVERY_SECOND3_(second, ...) second, EVERY_SECOND2(__VA_ARGS__)
#define EVERY_SECOND3(first, ...) EVERY_SECOND3_(__VA_ARGS__)

#define EVERY_SECOND4_(second, ...) second, EVERY_SECOND3(__VA_ARGS__)
#define EVERY_SECOND4(first, ...) EVERY_SECOND4_(__VA_ARGS__)

#define EVERY_SECOND5_(second, ...) second, EVERY_SECOND4(__VA_ARGS__)
#define EVERY_SECOND5(first, ...) EVERY_SECOND5_(__VA_ARGS__)

#define COUNT_EVERY_SECOND_(_1,__1,_2,__2,_3,__3,_4,__4,_5,__5,num,...) EVERY_SECOND##num
#define PARAMETER_NAMES(...) COUNT_EVERY_SECOND_(__VA_ARGS__,5,5,4,4,3,3,2,2,1,0)(__VA_ARGS__)

/* analogue to stackoverflow answer for parameter types */
#define EVERY_FIRST1(first, ...) first
#define EVERY_FIRST2(first, second, ...) first, EVERY_FIRST1(__VA_ARGS__)
#define EVERY_FIRST3(first, second, ...) first, EVERY_FIRST2(__VA_ARGS__)
#define EVERY_FIRST4(first, second, ...) first, EVERY_FIRST3(__VA_ARGS__)
#define EVERY_FIRST5(first, second, ...) first, EVERY_FIRST4(__VA_ARGS__)

#define COUNT_EVERY_FIRST_(_1,__1,_2, __2,_3, __3,_4, __4,_5, __5,num,...) EVERY_FIRST##num
#define PARAMETER_TYPES(...) COUNT_EVERY_FIRST_(__VA_ARGS__, 5,5,4,4,3,3,2,2,1,1,0)(__VA_ARGS__)

/* analogue to stackoverflow answer for function signatures */
#define ALL1(first, second, ...) first second
#define ALL2(first, second, ...) first second, ALL1(__VA_ARGS__)
#define ALL3(first, second, ...) first second, ALL2(__VA_ARGS__)
#define ALL4(first, second, ...) first second, ALL3(__VA_ARGS__)
#define ALL5(first, second, ...) first second, ALL4(__VA_ARGS__)

#define COUNT_ALL_(_1, __1, _2, __2, _3, __3, _4, __4, _5, __5, num, ...) ALL##num
#define FUNCTION_SIGNATURE(...) COUNT_ALL_(__VA_ARGS__, 5,5,4,4,3,3,2,2,1,1,0)(__VA_ARGS__)
