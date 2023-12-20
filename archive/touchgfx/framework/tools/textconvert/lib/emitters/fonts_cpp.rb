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
class FontsCpp
  def self.font_convert=(font_convert)
    @@font_convert = font_convert
  end

  def initialize(text_entries, typographies, output_directory, font_asset_path, data_format)
    @typographies = typographies
    @output_directory = output_directory
    @font_asset_path = font_asset_path
    @data_format = data_format
  end
  def run
    unique_typographies = @typographies.map{ |t| Typography.new("", t.font_file, t.font_size, t.bpp, t.fallback_character) }.uniq

    unique_typographies.sort_by { |t| sprintf("%s %04d %d",t.font_file,t.font_size,t.bpp) }.each do |typography|
      fonts_directory = File.expand_path(@output_directory)
      font_file = File.expand_path("#{@font_asset_path}/#{typography.font_file}")
      fallback_char = typography[:fallback_character]
      fallback_char ||= 0
      cmd = "#{@@font_convert} \
-f #{font_file} \
-w #{typography.font_size} \
-r #{typography.font_size} \
-o #{fonts_directory} \
-c \"#{@output_directory}/UnicodeList#{typography.cpp_name}_#{typography.font_size}_#{typography.bpp}.txt\" \
-n #{typography.cpp_name} \
-b #{typography.bpp} \
-d #{fallback_char} \
-#{@data_format}"

      output = `#{cmd}`
      if !$?.success?
        puts cmd
        puts output
        raise "Error generating font from #{font_file}"
      elsif output.match(/WARNING/i)
        puts output
      end
    end
  end
end
