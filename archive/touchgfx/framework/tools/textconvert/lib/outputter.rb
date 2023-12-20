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
require 'lib/file_io'
require 'lib/emitters/template'
require 'lib/emitters/text_keys_and_languages_hpp'
require 'lib/emitters/texts_cpp'
require 'lib/emitters/languages_cpp'
require 'lib/emitters/unicodes_txt'
require 'lib/emitters/fonts_cpp'
require 'lib/emitters/application_font_provider_hpp'
require 'lib/emitters/application_font_provider_cpp'
require 'lib/emitters/typed_text_database_hpp'
require 'lib/emitters/typed_text_database_cpp'

class Outputter
  def initialize(text_entries, typographies, localization_output_directory, fonts_output_directory, font_asset_path, data_format)
    @text_entries = text_entries
    @typographies = typographies
    @localization_output_directory = localization_output_directory
    @fonts_output_directory = fonts_output_directory
    @font_asset_path = font_asset_path
    @data_format = data_format
  end

  def run

    [ UnicodesTxt,
      ApplicationFontProviderCpp,
      ApplicationFontProviderHpp ].each { |template| template.new(@text_entries, @typographies, @fonts_output_directory).run }

    [ TextKeysAndLanguages,
      TextsCpp,
      LanguagesCpp,
      TypedTextDatabaseHpp,
      TypedTextDatabaseCpp ].each { |template| template.new(@text_entries, @typographies, @localization_output_directory).run }

    FontsCpp.new(@text_entries, @typographies, @fonts_output_directory, @font_asset_path, @data_format).run

  end
end

