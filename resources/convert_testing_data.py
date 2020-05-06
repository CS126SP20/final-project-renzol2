import csv

"""
This script is used to extract data from the comprehensive dataset of COVID-19 testing from Our World In Data.
Written by: Renzo Ledesma (renzol2@illinois.edu)

The dataset can be found on their Github page.
https://github.com/owid/covid-19-data/blob/master/public/data/testing/covid-testing-all-observations.csv

More information about their Coronavirus source data can be found here:
https://ourworldindata.org/coronavirus-source-data

In more detail, this script takes the data from Our World In Data (OWID)'s dataset on testing
and extracts individual datasets formatted more like their CSV datasets for general COVID-19 information
taken from the European Centre for Disease Prevention and Control (ECDC). Those datasets can be found here:
https://github.com/owid/covid-19-data/tree/master/public/data/ecdc

I had originally written my project to only handle their ECDC files, but I had enough time and
ambition to include their testing datasets as well.

Ideally I would implement this conversion from one CSV format to another in the project itself using
C++ and the capabilities of Cinder, but due to time constraints, my lack of comfort with file handling
in C++, and my larger relative comfort working in Python for this sort of work, I simply decided to use
Python and include this script in my project repo.

Formal source citation:
*Hannah Ritchie (2019) - "Coronavirus Source Data". Published online at OurWorldInData.org. Retrieved from: 'https://ourworldindata.org/coronavirus-source-data' [Online Resource]*
"""

path = ''  # folder path goes here

file = open( path +"covid-testing-all-observations.csv", "r", encoding='utf-8')
reader = csv.reader(file)

# Original file header:
# Entity,ISO code,Date,Source URL,Source label,Notes,Cumulative total,Daily change in cumulative total,Cumulative total per thousand,Daily change in cumulative total per thousand,3-day rolling mean daily change,3-day rolling mean daily change per thousand,7-day rolling mean daily change,7-day rolling mean daily change per thousand
region_index = 0
date_index = 2

cumulative_total = 6
daily_change_in_total = 7
cumulative_total_p_thousand = 8
daily_change_in_total_p_thousand = 9

dates = []
regions = []

for line in reader:
  # First get dates
  date = line[date_index]
  if len(date) != 10:  # skip the header
    continue
  if date not in dates:
    dates.append(date)
  
  # Then get regions
  region = line[region_index]
  if region not in regions:
    regions.append(region)

unique_dates = list(set(dates))
unique_regions = list(set(regions))

unique_dates.sort()
unique_regions.sort()

# Print statements to check that dates/regions are sorted correctly
# print(unique_dates)
# print(unique_regions)

# Converted file header
# Date, country1, country2, country3, ...

# Example of writing file for cumulative total tests:
# CUMULATIVE TOTAL TESTS FILE
with open('cumulative_total_tests.csv', mode='w', newline='') as cumulative_total_tests_file:
  writer = csv.writer(cumulative_total_tests_file, delimiter=',', quotechar='"')

  # Begin writing: add header first
  header = ['Date'] + unique_regions
  writer.writerow(header)

  # Then begin writing data from old file to new file in desired format
  for date in unique_dates:
    # Start the new line as a list
    newline = [date]
    for region in unique_regions:
      file.seek(0)
      found_data = False

      for line in reader:
        # Go over the old file and find any region/date matches, and add to list
        if line[date_index] == date and line[region_index] == region:
          newline.append(line[cumulative_total])  # I would change the index of line to be whatever data I needed
          found_data = True
        if found_data:
          break
      
      # Every region must have some entry, even if empty
      if not found_data:
        newline.append('')
    writer.writerow(newline)
