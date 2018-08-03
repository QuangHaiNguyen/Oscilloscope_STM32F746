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

class LanguagesCpp
  def initialize(text_entries, typographies, output_directory)
    @text_entries = text_entries
    @output_directory = output_directory
  end
  def run
    @text_entries.languages.each do |language|
      LanguageXxCpp.new(@text_entries, @output_directory, language).run
    end
  end
end

class LanguageXxCpp < Template
  Presenter = Struct.new(:text_id, :translation, :unicodes)

  def initialize(text_entries, output_directory, language)
    @language = language
    super(text_entries, [], output_directory)
  end

  def language
    @language
  end

  def entries
    entries = text_entries

    entries = handle_no_entries(entries, "DO_NOT_USE")
    present(entries)
  end

  def entries_texts_const_initialization
    entries.map { |entry| "    #{entry.text_id}_#{language}" }.join(",\n")
  end

#  def entries_s
#    entries = text_entries.entries_with_1_substitution
#    entries = handle_no_entries(entries, "DO_NOT_USE_S")
#    present(entries)
#  end

# def entries_s_texts_const_initialization
#   entries_s.map { |entry| "#{entry.text_id}_#{language}" }.join(",\n")
# end

# def entries_ss
#   entries = text_entries.entries_with_2_substitutions
#   entries = handle_no_entries(entries, "DO_NOT_USE_SS")
#   present(entries)
# end

# def entries_ss_texts_const_initialization
#   entries_ss.map { |entry| "#{entry.text_id}_#{language}" }.join(",\n")
# end

  def input_path
    'Templates/LanguageXX.cpp.temp'
  end

  def output_path
    "src/Language#{language}.cpp"
  end

  private

  def handle_no_entries(entries, text)
    if entries.empty?
       empty_entry = TextEntry.new(text, "typography")
       empty_entry.add_translation(language, "")
       [empty_entry]
    else
      entries
    end
  end

  def present(entries)
    entries.map do |entry|
      Presenter.new(entry.cpp_text_id, entry.translation_in(language).to_cpp, ( entry.translation_in(language).unicodes.map { |u| '0x' + u.to_s(16) } << '0x0' ) .join(', ') )
    end
  end

end
