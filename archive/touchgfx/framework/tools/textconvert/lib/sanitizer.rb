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
TextEntrySanitizer = Struct.new(:text_entries, :typographies)

$warning_prefix = "\nWARNING (TextConverter): "

class Sanitizer < TextEntrySanitizer

  def run
    [ RemoveDuplicateKeys,
      RemoveIncompleteLanguages,
      RemoveKeysWithMoreThanTwoSubstitutions,
      RemoveKeysWithDifferentNumberOfSubstitutions,
      RemoveTextEntriesWithInvalidTypography,
      RemoveTextEntriesWithInvalidAlignment,
      RemoveTextEntriesWithInvalidDirection,
      CheckSizeAndBpp
    ].each do |sanitizer|
      sanitizer.new(text_entries, typographies).run
    end
  end
end

class RemoveDuplicateKeys < TextEntrySanitizer
  def run
#        puts "Removing duplicate text ids"
    counts = Hash.new(0)
    counts = text_entries.inject(Hash.new(0)) do |h, entry|
      h[entry.cpp_text_id.upcase] = h[entry.cpp_text_id.upcase] + 1
      h
    end

    text_entries.each do |text_entry|
      if counts[text_entry.cpp_text_id.upcase] > 1
        raise "#{$warning_prefix} Duplicate key removed: #{text_entry.text_id}, yields cpp identifier #{text_entry.cpp_text_id.upcase}"
        text_entries.remove(text_entry)
      end
    end
  end
end

class RemoveIncompleteLanguages < TextEntrySanitizer
  def run
#       puts "Removing incomplete languages"
    languages = text_entries.languages
    languages.each do |language|
      text_entries_with_missing_translations = text_entries.select do |text_entry|
        text_entry.translation_in(language).empty?
      end
      text_entries_with_missing_translations.each do |text_entry|
        raise "#{$warning_prefix} Language #{language} is missing translation for #{text_entry.text_id}"
      end
      if text_entries_with_missing_translations.any?
        text_entries.remove_language(language)
      end
    end
  end
end

class RemoveKeysWithMoreThanTwoSubstitutions < TextEntrySanitizer
  def run
#        puts "Removing text entries with more than two substitutions"
    text_entries.languages.each do |language|
      text_entries_with_more_than_two_substitutions = text_entries.select do |text_entry|
        text_entry.number_of_substitutions_in(language) > 2
      end
      text_entries_with_more_than_two_substitutions.each do |text_entry|
        raise "#{$warning_prefix}  Text id #{text_entry.text_id} has #{text_entry.number_of_substitutions_in(language)} substitutions"
        #text_entries.remove(text_entry)
      end
    end
  end
end

class RemoveKeysWithDifferentNumberOfSubstitutions < TextEntrySanitizer
  def run
#        puts "Removing text entries with different number of substitutions"
    text_entries.each do |text_entry|
      translations = text_entry.translations
      number_of_substitutions_per_translation = translations.collect { |translation| translation.number_of_substitutions }
      if number_of_substitutions_per_translation.uniq.count > 1
        raise "#{$warning_prefix} Text id #{text_entry.text_id} has different number of substitutions for some languages"
        #text_entries.remove(text_entry)
      end
    end
  end
end

class RemoveTextEntriesWithInvalidTypography < TextEntrySanitizer
  def run
#        puts "Removing text entries referring to non existing typographies"
    text_entries.each do |text_entry|
      non_existing_typographies = (text_entry.get_all_typographies - typographies.map( &:name )).compact;

      if non_existing_typographies.any?
        raise "#{$warning_prefix} Text id #{text_entry.text_id} uses unknown typographies #{non_existing_typographies}"
        #text_entries.remove(text_entry)
      end
    end
  end
end

class RemoveTextEntriesWithInvalidAlignment < TextEntrySanitizer
  def run
    #    puts "Removing text entries referring to invalid alignment"
    text_entries.each do |text_entry|
      alignments = text_entry.get_all_alignments_as_string
      illegal_alignments = alignments.select { |a| !['LEFT', 'RIGHT', 'CENTER'].include?(a) }
      if illegal_alignments.any?
        raise "#{$warning_prefix} Text id #{text_entry.text_id} uses unknown alignments #{illegal_alignments}"
        #text_entries.remove(text_entry)
      end
    end
  end
end

class RemoveTextEntriesWithInvalidDirection < TextEntrySanitizer
  def run
    #    puts "Removing text entries referring to invalid direction"
    text_entries.each do |text_entry|
      directions = text_entry.get_all_directions_as_string
      illegal_directions = directions.select { |d| !['LTR', 'RTL'].include?(d) }
      if illegal_directions.any?
        raise "#{$warning_prefix} Text id #{text_entry.text_id} uses unknown directions #{illegal_directions}"
        #text_entries.remove(text_entry)
      end
    end
  end
end

class CheckSizeAndBpp < TextEntrySanitizer
  def run
#        puts "Raise an error if typography size or bpp does have sane values"
    typographies.each do |typography|
      if (not [1, 2, 4, 8].include?(typography.bpp))
        raise "#{$warning_prefix} Typography named '#{typography.name}' has bpp value '#{typography.bpp}', which is not a valid value"
      end

      if ( (not typography.font_size.integer?) or (typography.font_size < 1) )
        raise "#{$warning_prefix} Typography named '#{typography.name}' has font size value '#{typography.font_size}', which is not a valid value"
      end
    end
  end
end
