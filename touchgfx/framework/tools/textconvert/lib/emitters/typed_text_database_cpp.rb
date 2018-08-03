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
class TypedTextDatabaseCpp < Template
  TypedTextPresenter = Struct.new(:alignment, :direction, :typography)

  def typed_texts(language)
    text_entries.collect do |entry|
        typography_name = entry.typographies[language] || entry.typography
        typography = typographies.find { |t| t.name == typography_name }
        alignment = entry.alignments[language] || entry.alignment
        direction = entry.directions[language] || entry.direction
        TypedTextPresenter.new(alignment, direction, typography);
    end
  end

  def layouts
    if text_entries.empty?
      ["DEFAULT"]
    else
      (text_entries.first.typographies.keys + text_entries.first.alignments.keys + text_entries.first.directions.keys << "DEFAULT").uniq
    end
  end

  def fonts
   typographies.map{ |t| Typography.new("", t.font_file, t.font_size, t.bpp) }.uniq.collect do |f|
      "getFont_#{f.cpp_name}_#{f.font_size}_#{f.bpp}bpp"
    end
  end

  def fontmap
    fontmap = Hash.new
    fonts.each_with_index do |f, i|
      fontmap[f] = i
    end
    fontmap
  end

  def input_path
    'Templates/TypedTextDatabase.cpp.temp'
  end
  def output_path
    'src/TypedTextDatabase.cpp'
  end
end
