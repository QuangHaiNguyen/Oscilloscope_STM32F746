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
class TextEntries
  include Enumerable

  def initialize
    @entries = []
    @is_rtl = false
    @unicode_is_rtl = false
  end

  def each(&block)
    @entries.each(&block)
  end

  def remove(entry)
    @entries.delete(entry)
  end

  def add(entry)
    @entries.push(entry)
  end

  def empty?
    @entries.empty?
  end

  def different_typographies
    [ar,default]
  end

  def different_alignments
    [ar,default]
  end

  def languages
    if @entries.empty?
      []
    else
      @entries.first.languages
    end
  end

  def remove_language(language)
    @entries.each { |entry| entry.remove_translation_in(language) }
  end

  def typographies
    @entries.map { |entry| entry.typography }.uniq
  end

  def entries
    @entries
  end

  def with_typography(typography)
    @entries.select { |entry| entry.typography == typography }
  end

  def include?(text_entry)
    @entries.find { |entry| entry.text_id == text_entry.text_id || entry.cpp_text_id == text_entry.cpp_text_id }
  end

  def unicode_uses_rtl
    @unicode_is_rtl = true
  end

  def is_rtl
    @unicode_is_rtl || @entries.any? { |entry| entry.is_rtl }
  end

end

class TextEntry
  attr_reader :text_id
  attr_reader :typography
  attr_reader :typographies
  attr_reader :alignments
  attr_reader :directions

  def initialize(text_id, typography, alignment, direction)
    @text_id = text_id
    @typographies = {}
    @alignments = {}
    @directions = {}
    @translations = {}

    # default typography
    @typography = typography

    # default alignment
    @alignment = alignment

    # default direction
    @direction = get_direction_as_string(direction)
    @right_to_left = false
  end

  def add_typography(language, typography)
    @typographies[language] = typography
  end
  def add_alignment(language, alignment)
    @alignments[language] = alignment
  end
  def add_direction(language, direction)
    @directions[language] = direction
  end

  def add_translation(language, text)
    translation = Translation.new(text)
    @translations[language] = translation
  end

  def remove_translation_in(language)
    @translations.delete(language)
  end

  def translations
    @translations.values
  end

  def translation_in(language)
    @translations[language]
  end

  def translations_with_typography(typography)
    languages_with_typography = languages.select do |language|
      if @typographies[language].nil?
        @typography == typography
      else
        @typographies[language] == typography
      end
    end

    languages_with_typography.collect{ |language| translation_in(language) }
  end

  def languages
    @translations.keys
  end

  def number_of_substitutions_in(language)
    @translations[language].number_of_substitutions
  end

  def cpp_text_id
    cppify(text_id)
  end

  def alignment
    get_alignment_as_string(@alignment)
  end

  def direction
    get_direction_as_string(@direction)
  end

  # includes the default typography
  def get_all_typographies
    @typographies.values.compact.insert(0, @typography)
  end

  # includes the default alignment
  def get_all_alignments_as_string
    @alignments.values.compact.collect{ |a| get_alignment_as_string(a) }.insert(0, alignment)
  end

  # includes the default direction
  def get_all_directions_as_string
    @directions.values.compact.collect{ |a| get_direction_as_string(a) }.insert(0, direction)
  end

  def is_rtl
    @is_rtl
  end

  private

  def get_alignment_as_string(a)
    case a.to_s.downcase
    when 'right'
      'RIGHT'
    when 'center'
      'CENTER'
    when 'left', ''
      'LEFT'
    else
      a.to_s
    end
  end

  def get_direction_as_string(d)
    case d.to_s.downcase
    when 'ltr', ''
      'LTR'
    when 'rtl'
      @is_rtl = true
      'RTL'
    else
      d.to_s
    end
  end

  def cppify(text)
    t_type = "T_" + text

    # strip the keys for characters, that can not be used in C++
    t_type = t_type.to_ascii
    t_type.gsub!(" ", "_")
    t_type.gsub!(")", "")
    t_type.gsub!("(", "")
    t_type.gsub!("-", "")
    t_type.gsub!("\"", "")
    t_type.gsub!("/", "")
    t_type.gsub!(".", "")
    t_type
  end
end

class Translation
  def initialize(text)
    @text = text
  end
  def empty?
    @text.nil? || @text.empty?
  end
  def number_of_substitutions
    @text.scan(/<.*?>/).length
  end
  def unicodes
    numbers.map { |number| number.to_s.gsub(/\[|\]/,'').to_i }
  end
  def to_cpp
    @text.
      # Remove all ascii(2) from the string
      gsub("\2", "").
      # Convert '<foo>' to '\u0002' and '\<bar\>' to '<bar>'
      gsub(/([^\\]|^)<.+?>/, '\1'+"\2").gsub('\\<', '<').gsub('\\>', '>')
  end
  private
  def numbers
    to_cpp.unpack('U*')
  end
end


class String
  def to_ascii
    # modernized version of http://craigjolicoeur.com/blog/ruby-iconv-to-the-rescue
    self.encode("ASCII", "UTF-8", :undef => :replace, :invalid => :replace, :replace => '').
      unpack('U*').select { |cp| cp < 127 }.pack('U*')
  end
end
