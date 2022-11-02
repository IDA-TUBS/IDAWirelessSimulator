-- Coding Guidelines --
=======================

-- Naming Convention --
=======================
Use meaningful names so everyone is able to understand their purpose

## Variable Names
    Variables start with a lower-case letter and use upper-case letters as separators. Do not use underbars ('_')
	   Example:
			int myVariable;

## Constant Names
    Constants are written with capital letters and use underbars ('_') as separators
    Example:
			const int MY_CONSTANT = 100;

## Function Names
	Use the imperative form of verbs to name the functions. Two options are possible:
	Start with a lower-case letter and upper-case letters as separators. Do not use underbars ('_') 
	   Example:
			int doSomething()

## Class Names
	Start with an upper-case letter and use upper-case letters as separators. Do not use underbars ('_')
	Example:
		class MyClassName
		
## Module Names
	Start with an lower-case letter and use upper-case letters as separators. Do not use underbars ('_')
	Example:
		myModuleName
		
## Module Names
	?

-- Comments --
==============
Use doxygen comment style. These comments are only in the header-file (http://www.doxygen.nl/manual/index.html)
	Example:
		/**
		 * This is a description of the class/function/struct...
		 *
		 * @author [name]
		 * @version [version number and date]
		 * @param [variable_name] for each function variable one @param is required with short description
		 * @returns return value description of a function
		 * @note a note for users
		 * @attention ...
		 * @warning
		 */
		 
		 ///this is a one line doxygencomment 
		 int myVariable;

-- Curly Braces --
==================
Place the braces under and inline with the keywords, function, etc.
	Example:
		if (condition)
		{
			...
		}
>> Attention: all if, while and do statements must have braces. They must not be on a single line. <<

-- White Spaces --
==================
## Tab Characters
	No Tab Characters. Use 4 white spaces instead

## Line Length
	Limit the line length to about 80 characteres. This makes it easier to read the program code

## Spacing Around Operators
	To improve the readability of expressions put spaces before and after operators
	Example:
		int a = b + c;

## Indentation
	Always indent with curly braces. Use 4 white spaces for each indentation level
	Example:
		int do_something()
        {
		    int a = 1;
	        int b = 2;
		    if ( a == b)
			{
				int c = a - b;
			}
			return a;
		}

## if-else formatting
	Place brace under und inline with keywords
	Example:
		if (condition)					\\ comment
		{
			...
		}
		else if (condition)				\\ comment
		{
			...
		}
		else							\\ comment
		{
			...
		}
