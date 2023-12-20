###############################################################################
#
# @brief     This file is part of the TouchGFX 4.7.0 evaluation distribution.
#
# @author    Draupner Graphics A/S <http://www.touchgfx.com>
#
###############################################################################
#
# @section Copyright
#
# Copyright (C) 2014-2016 Draupner Graphics A/S <http://www.touchgfx.com>.
# All rights reserved.
#
# TouchGFX is protected by international copyright laws and the knowledge of
# this source code may not be used to write a similar product. This file may
# only be used in accordance with a license and should not be re-
# distributed in any way without the prior permission of Draupner Graphics.
#
# This is licensed software for evaluation use, any use must strictly comply
# with the evaluation license agreement provided with delivery of the
# TouchGFX software.
#
# The evaluation license agreement can be seen on www.touchgfx.com
#
# @section Disclaimer
#
# DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Draupner Graphics A/S has
# no obligation to support this software. Draupner Graphics A/S is providing
# the software "AS IS", with no express or implied warranties of any kind,
# including, but not limited to, any implied warranties of merchantability
# or fitness for any particular purpose or warranties against infringement
# of any proprietary rights of a third party.
#
# Draupner Graphics A/S can not be held liable for any consequential,
# incidental, or special damages, or any other relief, or for any claim by
# any third party, arising from your use of this software.
#
###############################################################################
$:.unshift File.dirname(__FILE__)
require 'rubygems'
require 'lib/generator'
require 'lib/emitters/fonts_cpp'

WINDOWS_LINE_ENDINGS = "\r\n"
UNIX_LINE_ENDINGS = "\n"
#on windows/mingw file.write will it self translate \n to \r\n, on linux not
LINE_ENDINGS = RUBY_PLATFORM.match(/linux/) ? WINDOWS_LINE_ENDINGS : UNIX_LINE_ENDINGS

FileEntry = Struct.new(:cpp_file, :content, :texts_name, :texts, :extern)
StringEntry = Struct.new(:cpp_file, :t_symbol, :hex_values)
Database = Struct.new(:pragma, :name, :value)
DatabaseArray = Struct.new(:pragma, :databases, :finish)

class Main
    def self.banner
        <<-BANNER
Create cpp text files from excel translations
              
Usage: #{File.basename($0)} file.xlsx path/to/fontconvert.out path/to/fonts_output_dir path/to/localization_output_dir path/to/font/asset remap_identical_texts data_format
Where 'remap_identical_texts' is yes/no
      'data_format' is A4/A8 or blank
BANNER
    end

    def self.save_int_flash(localization_output_path)
      local_path = localization_output_path.gsub('\\','/')
      # Handle Languge*.cpp
      languages_cpp = Dir["#{local_path}/src/Language*.cpp"]
      files = languages_cpp.collect do |cpp|
        content = IO.binread(cpp)
        texts_array = content.match(/^{(.*)^}/m)[1].gsub(/\s+/,'').split(',')
        texts_name = content.match(/^KEEP extern const touchgfx::Unicode::UnicodeChar\* const (texts.*)\[/)[1]
        FileEntry.new(cpp,content,texts_name,texts_array,[])
      end
      strings = files.collect do |file|
        file.content.scan(/UnicodeChar (T_.*)\[.*{ ([0-9a-fA-Fx, ]*) }/).collect { |t_symbol, hex_values| StringEntry.new(file.cpp_file, t_symbol, hex_values) }
      end.flatten.sort_by { |x| sprintf("%010d %s %s",x.hex_values.count(','),x.cpp_file,x.t_symbol) }
      remapped_strings = 0
      remapped_bytes = 0
      strings.each_with_index do |short,short_ix|
        short_in_long_ix=nil
        long_ix = strings.rindex do |long|
          short_in_long_ix = long.hex_values.index(short.hex_values)
        end
        if long_ix > short_ix
          remapped_strings += 1
          remapped_bytes += (short.hex_values.count(',') + 1) * 2
          long = strings[long_ix]
          srch = short.t_symbol
          rplc = long.t_symbol
          offset = long.hex_values[0..short_in_long_ix].count(',')
          ofst = offset > 0 ? "+#{offset}" : ""
          file = files.find { |file| file.cpp_file == short.cpp_file }
          # Remove the 'KEEP' declaration to prevent the symbol from staying in memory even if unreferenced
          file.content.gsub!(/TEXT_LOCATION_FLASH_PRAGMA\r\nKEEP const touchgfx::Unicode::UnicodeChar #{srch}\[.*?;\r\n/,'')
          file.texts = file.texts.map { |s| s == srch ? "#{rplc}#{ofst}" : s }
          if short.cpp_file != long.cpp_file
            file.extern = ( file.extern + [ rplc ] ).uniq
          end
          file = files.find { |file| file.cpp_file == long.cpp_file }
          if short.cpp_file != long.cpp_file
            file.extern = ( file.extern + [ rplc ] ).uniq
          end
        end
      end
      puts "Remapped #{remapped_strings} strings (#{remapped_bytes} bytes saved)" if remapped_strings > 0

      # Handle Texts.cpp
      texts_cpp = "#{local_path}/src/Texts.cpp"
      texts = IO.binread(texts_cpp)
      files.each_with_index do |srch,i|
        j = files.rindex { |f| srch.texts == f.texts }
        if j > i
          rplc = files[j]
          texts.gsub!(/^extern const touchgfx::Unicode::UnicodeChar\* const #{rplc.texts_name}\[\];\r\n/,'')
          texts.gsub!(/^( *)#{rplc.texts_name}(,|$)/,"\\1#{srch.texts_name}\\2")
          rplc.texts = []
          puts "Remapped #{rplc.texts_name}"
        end
      end
      # Handle TypedTextDatabase.cpp
      typed_text_database_cpp = "#{local_path}/src/TypedTextDatabase.cpp"
      typed_text_database = IO.binread(typed_text_database_cpp)
      databases = typed_text_database.scan(/(TEXT_LOCATION_FLASH_PRAGMA\r\nconst touchgfx::TypedText::TypedTextData )(typedText_database_.*?)(\[\].*? = {.*?};\r\n\r\n)/m).collect do |pragma,name,value|
        Database.new(pragma, name, value)
      end
      pragma,dbs,finish = typed_text_database.match(/(TEXT_LOCATION_FLASH_PRAGMA\r\nconst touchgfx::TypedText::TypedTextData\* const typedTextDatabaseArray\[\] TEXT_LOCATION_FLASH_ATTRIBUTE =\r\n{\r\n)(.*?)(\r\n};)/m).captures
      database_array = DatabaseArray.new(pragma, dbs.gsub(/[ \r\n]/,'').split(','), finish)
      header,footer = typed_text_database.match(/(.*?)TEXT_LOCATION_FLASH_PRAGMA.*?(\r\n\r\nnamespace.*)/m).captures
      default_ix = databases.find_index { |db| db.name.match(/_DEFAULT/) }
      # Move _DEFAULT to start of array
      databases.unshift(databases.delete_at(default_ix))
      # Remove duplicates
      databases.each do |rplc|
        databases.each_with_index do |srch, ix|
          if srch.value == rplc.value && srch.name != rplc.name
            puts "Remapped #{srch.name}"
            database_array.databases.map! do |db|
              db == srch.name ? rplc.name : db
            end
            databases.delete_at(ix)
          end
        end
      end

      # Write modified files back

      # Texts.cpp
      File.open(texts_cpp,'wb') { |f| f.write(texts) }

      # TypedTextDatabase.cpp
      File.open(typed_text_database_cpp,'wb') do |f|
        f.write(header)
        databases.each do |db|
          f.write(db.pragma)
          f.write(db.name)
          f.write(db.value)
        end
        f.write("\r\n")
        f.write(database_array.pragma)
        f.write(database_array.databases.map{|x|"    #{x}"}*",\r\n")
        f.write(database_array.finish)
        f.write(footer)
      end

      # Language*.cpp
      files.each do |file|
        if file.texts.length > 0
          parts = file.content.match(/(.*#endif).*(\/\/ Language.*^{).*(}.*)/m)
          newmiddle = file.texts * ",\r\n"
          externs = file.extern.collect{|e|"extern const touchgfx::Unicode::UnicodeChar #{e}[];"}*"\r\n"
          File.open(file.cpp_file,'wb') { |f| f.write("#{parts[1]}\r\n\r\n#{externs}\r\n\r\n#{parts[2]}\r\n#{newmiddle}\r\n#{parts[3]}") }
        else
          parts = file.content.match(/(.*#endif).*(\/\/ Language.*)^TEXT_LOCATION_FLASH_PRAGMA\r\nKEEP extern const touchgfx::Unicode::UnicodeChar\* const.*/m)
          externs = file.extern.collect{|e|"extern const touchgfx::Unicode::UnicodeChar #{e}[];"}*"\r\n"
          File.open(file.cpp_file,'wb') { |f| f.write("#{parts[1]}\r\n\r\n#{externs}\r\n\r\n#{parts[2]}\r\n") }
        end
      end
      
    end

    if __FILE__ == $0
        if ARGV.count < 7 || ARGV.count > 8
            abort(self.banner)
        end      
        file_name = ARGV.shift
        FontsCpp.font_convert = ARGV.shift
        fonts_output_path = ARGV.shift
        localization_output_path = ARGV.shift
        font_asset_path = ARGV.shift
        $calling_path = ARGV.shift
        remap_identical_texts = ARGV.shift
        data_format = ARGV.shift
        if remap_identical_texts=='A4' || remap_identical_texts=='A8'
          data_format,remap_identical_texts = remap_identical_texts,data_format
        end
        begin
          Generator.new.run(file_name, fonts_output_path, localization_output_path, font_asset_path, data_format)

          if remap_identical_texts && remap_identical_texts.downcase == 'yes'
            self.save_int_flash(localization_output_path)
          end
        rescue Exception => e
            # Makefiles depend on this file, so remove in case of error.
            FileUtils.rm_f "#{localization_output_path}/include/texts/TextKeysAndLanguages.hpp"
            abort("an error occurred in converting texts:\r\n#{e}")
        end
    end
end
