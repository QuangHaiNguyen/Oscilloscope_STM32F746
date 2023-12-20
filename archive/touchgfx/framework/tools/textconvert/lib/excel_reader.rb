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
require 'roo'
require 'lib/text_entries'

class ExcelRow

  def initialize(excel_file, header, row_number, first_column)
    @excel_file = excel_file
    @header = header
    @row_number = row_number
    @first_column = first_column
  end

  def [](column_header)
    value_at(@row_number, column_number(column_header.to_s))
  end

  def exists?(name)
    column_index = @header.find_index { |val| val.downcase == name.to_s.downcase }
    !column_index.nil?
  end

  private

  def column_number(name)
    column_index = @header.find_index { |val| val.downcase == name.downcase }
    if column_index.nil?
      raise "#{name} column not found in excel file"
    end
    column_index + @first_column
  end

  def value_at(row, col)
    if @excel_file.empty?(row,col)
      nil
    else
      value = @excel_file.cell(row,col).to_s
      check_encoding(value)
      value
    end
  end

  def check_encoding(value)
    puts value if value.force_encoding("UTF-8").valid_encoding? == false
  end

end

class ExcelReader

  def initialize(file_name, sheet, header_row, first_column)
    @excel_file = Roo::Excelx.new file_name
    @sheet = sheet
    @excel_file.default_sheet = sheet
    @header_row = header_row
    @first_column = first_column
  end

  def read_header
    yield header
  end

  def read_rows
    (@header_row + 1).upto(last_row_number) do |row_number|
      yield row(row_number)
    end
  end

  private

  def last_row_number
    @excel_file.last_row
  end

  def header
    @excel_file.row(@header_row).compact.map(&:strip)
  end

  def row(row_number)
    ExcelRow.new(@excel_file, header, row_number, @first_column)
  end

end
