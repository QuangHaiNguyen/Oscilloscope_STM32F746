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
class UnicodesTxt
  def initialize(text_entries, typographies, output_directory)
    @text_entries = text_entries
    @typographies = typographies
    @output_directory = output_directory
  end
  def run
    unique_typographies = @typographies.map{ |t| Typography.new("", t.font_file, t.font_size, t.bpp) }.uniq.sort_by { |t| sprintf("%s %04d %d",t.font_file,t.font_size,t.bpp) }
    unique_typographies.each do |unique_typography|
      UnicodeForTypographyTxt.new(@text_entries, @output_directory, @typographies, unique_typography).run
    end
  end
end

class UnicodeForTypographyTxt
  def initialize(text_entries, output_directory, typographies, unique_typography)
    @text_entries = text_entries
    @output_directory = output_directory
    @typographies = typographies
    @unique_typography = unique_typography
  end
  def convert_to_contextual_forms(unicodes)
    @contextutal_forms ||= { 0x0621=>[0xFE80],
                             0x0622=>[0xFE81,0xFE82],
                             0x0623=>[0xFE83,0xFE84],
                             0x0624=>[0xFE85,0xFE86],
                             0x0625=>[0xFE87,0xFE88],
                             0x0626=>[0xFE89,0xFE8A,0xFE8B,0xFE8C],
                             0x0627=>[0xFE8D,0xFE8E],
                             0x0628=>[0xFE8F,0xFE90,0xFE91,0xFE92],
                             0x0629=>[0xFE93,0xFE94],
                             0x062A=>[0xFE95,0xFE96,0xFE97,0xFE98],
                             0x062B=>[0xFE99,0xFE9A,0xFE9B,0xFE9C],
                             0x062C=>[0xFE9D,0xFE9E,0xFE9F,0xFEA0],
                             0x062D=>[0xFEA1,0xFEA2,0xFEA3,0xFEA4],
                             0x062E=>[0xFEA5,0xFEA6,0xFEA7,0xFEA8],
                             0x062F=>[0xFEA9,0xFEAA],
                             0x0630=>[0xFEAB,0xFEAC],
                             0x0631=>[0xFEAD,0xFEAE],
                             0x0632=>[0xFEAF,0xFEB0],
                             0x0633=>[0xFEB1,0xFEB2,0xFEB3,0xFEB4],
                             0x0634=>[0xFEB5,0xFEB6,0xFEB7,0xFEB8],
                             0x0635=>[0xFEB9,0xFEBA,0xFEBB,0xFEBC],
                             0x0636=>[0xFEBD,0xFEBE,0xFEBF,0xFEC0],
                             0x0637=>[0xFEC1,0xFEC2,0xFEC3,0xFEC4],
                             0x0638=>[0xFEC5,0xFEC6,0xFEC7,0xFEC8],
                             0x0639=>[0xFEC9,0xFECA,0xFECB,0xFECC],
                             0x063A=>[0xFECD,0xFECE,0xFECF,0xFED0],
                             0x0641=>[0xFED1,0xFED2,0xFED3,0xFED4],
                             0x0642=>[0xFED5,0xFED6,0xFED7,0xFED8],
                             0x0643=>[0xFED9,0xFEDA,0xFEDB,0xFEDC],
                             0x0644=>[0xFEDD,0xFEDE,0xFEDF,0xFEE0,0xFEF5,0xFEF6,0xFEF7,0xFEF8,0xFEF9,0xFEFA,0xFEFB,0xFEFC,0xFEFF], # The extras are for ligature LAM with ALEF and zero width space
                             0x0645=>[0xFEE1,0xFEE2,0xFEE3,0xFEE4],
                             0x0646=>[0xFEE5,0xFEE6,0xFEE7,0xFEE8],
                             0x0647=>[0xFEE9,0xFEEA,0xFEEB,0xFEEC],
                             0x0648=>[0xFEED,0xFEEE],
                             0x0649=>[0xFEEF,0xFEF0],
                             0x064A=>[0xFEF1,0xFEF2,0xFEF3,0xFEF4] }
    unicodes.collect{|u| @contextutal_forms.keys.include?(u) ? @contextutal_forms[u] : u}.flatten
  end
  def mirror_brackes(unicodes)
    [['<','>'],['(',')'],['[',']'],['{','}']].each do |l,r|
      has_l = unicodes.include?(l.ord)
      has_r = unicodes.include?(r.ord)
      unicodes.push(r.ord) if has_l and !has_r
      unicodes.push(l.ord) if has_r and !has_l
    end
    unicodes
  end
  def check_for_rtl(unicodes)
    return if @text_entries.is_rtl # No need to look for unicode if RTL already detected
    # Look for hebrew (0x0500-0x05ff) or arabic (0x0600-0x06ff) + arabic ligatures (0xFE70-0xFEFF)
    @text_entries.unicode_uses_rtl if unicodes.any?{|u| u.between?(0x0500, 0x05FF) || u.between?(0x0600, 0x06FF) || u.between?(0xFE70, 0xFEFF) }
  end
  def decode_ranges(str)
    result = []
    while str.length > 0
      char_range = str.match(/^(.)-(.)(.*)$/)
      if char_range
        first_char = char_range[1]
        last_char = char_range[2]
        result += (first_char.ord .. last_char.ord).to_a
        str = char_range[3]
      else
        num_range = str.match(/^(0[xX][0-9a-fA-F]+|\d+)(?:\.0+)?-(0[xX][0-9a-fA-F]+|\d+)(?:\.0+)?(.*)$/)
        if num_range
          first_num = Integer(num_range[1])
          last_num = Integer(num_range[2])
          result += (first_num..last_num).to_a
          str = num_range[3]
        else
          num = str.match(/^(0[xX][0-9a-fA-F]+|\d+)(?:\.0+)?(.*)/)
          if num
            result += [Integer(num[1])]
            str = num[2]
          else
            puts "Unexpected character at #{str}"
            exit
          end
        end
      end
      if str.length > 0
        if str[0] == ','
          str = str[1..-1]
        else
          puts "Please separate wildcard ranges with ','"
          exit
        end
      end
    end
    result
  end

  def run
    typographies_identical = @typographies.select{ |t| t.font_file == @unique_typography.font_file &&
                                                       t.font_size == @unique_typography.font_size &&
                                                       t.bpp == @unique_typography.bpp }
    typography_names = typographies_identical.map{ |t| t.name }.uniq

    # Find a typography with a fallback character
    typography_with_fallback_character = typographies_identical.find { |t| t.fallback_character }
    if typography_with_fallback_character
      # Now get the actual fallback character (or 'skip')
      typography_fallback_character = typography_with_fallback_character.fallback_character
      # Check to see if one of the other typographes has a different fallback character
      index = typographies_identical.find_index{ |t| t.fallback_character && t.fallback_character != typography_fallback_character }
      if index
        puts "The default character differs for typography \"#{typography_with_fallback_character.name}\" and typography \"#{typographies_identical[index].name}\""
        exit
      end
      # set all fallback characters to the same character
      typographies_identical.each { |t| t.fallback_character = typography_fallback_character }
    end

    all_translations = typography_names.map{ |typography_name| @text_entries.collect{ |entry| entry.translations_with_typography(typography_name) }.flatten }.flatten

    unicodes = all_translations.map(&:unicodes).flatten.uniq.sort

    typographies_identical.each do |t|
      fbc = t.fallback_character
      fbcUnicode = 0
      if fbc
        if fbc.downcase == 'skip'
          fbcUnicode = 0xFEFF
        elsif fbc.length == 1
          fbcUnicode = fbc[0].ord
        else
          begin
            fbcUnicode = Integer(fbc)
          rescue
            puts "Please only specify one character or ('skip') as default, typography \"#{typography_with_fallback_character.name}\" has default #{typography_with_fallback_character.fallback_character}"
            fail
          end
        end
        unicodes += [ fbcUnicode ]
      end
      t.fallback_character = fbcUnicode
    end
    typographies_identical.each{ |t|
      if t.wildcard_characters
        t.wildcard_characters.to_s.split('').each { |c|
          unicodes += [ c[0].ord ]
        }
      end
      if t.wildcard_ranges
        unicodes += decode_ranges(t.wildcard_ranges)
      end
    }

    unicodes = convert_to_contextual_forms(unicodes)

    unicodes = mirror_brackes(unicodes)

    unicodes = unicodes.uniq.sort
    
    check_for_rtl(unicodes)

    FileIO.write_file(File.join(@output_directory, "UnicodeList#{@unique_typography.cpp_name}_#{@unique_typography.font_size}_#{@unique_typography.bpp}.txt"), unicodes.join(LINE_ENDINGS) )
  end
end
