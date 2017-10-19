# UTF_CONVERTER

Supports:

         UTF8 to UTF16LE
         UTF8 to UTF16BE
         UTF16LE to UTF8
         UTF16BE to UTF8
         UTF16LE to UTF16BE
         UTF16BE to UTF16LE

Compile the codes by: make/make clean all

Run the codes with following format:
bin/utf -e ENCODING INPUT_FILE OUTPUT_FILE

ENCODING:
         
         MANDATORY FLAG: Choose output format.
         Accepted values:
         -UTF16LE
         -UTF16BE
         -UTF8

INPUT_FILE:

         File to convert.
         Must contain a Byte Order Marking (BOM)

OUTPUT_FILE:

         Output file.
         Will contain a Byte Order Marking (BOM)
