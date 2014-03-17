# Simple text processor #

Simple text processor which can process text files chunk by chunk without loading all file in memory.
This is console application. Processing performed using information in config file. 

## Building text processor ##

Clone this repository, `cd` into it and type `make`. After that you can run console app `build/main`
To clean simply use `make clean`.

## Running text processor ##

For launch:
```
    ./main [input_file_name] [output_file_name] [config_file_name]
```

## Text processing rules ##

In config file you can use following rules.
First word of the rule can be `word` or `prefix`.
To replace words:

```
    word/prefix <word_in_text> replace <word_in_text>
```

To remove words:

```
    word/prefix <word_in_text> remove word/sentence
```





