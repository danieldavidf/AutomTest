# Paramenter < ParamRange < TestSet < Method

class Parameter:

	def __init__(self, name, type_name=''):
		self.name = name
		self.type_name = type_name

	def __str__(self):
		return self.name + " (" +self.type_name + ")"

# NEW CLASS ######################################################################

class ParamRange:

	def __init__(self, param, v1, v2='', v3=''):
		self.param = param
		self.v1 = v1
		self.v2 = v2
		self.v3 = v3

	def __str__(self):
		output = self.param.__str__()
		
		if (self.v2 == '' and self.v3 == ''):
			output += "\n\t" + self.v1
		elif (self.v3 == ''):
			output += "\n\t" + self.v1 + " / " + self.v2
		else:
			output += "\n\t" + self.v1 + " / " + self.v2 + " / " + self.v3
		
		return output

# NEW CLASS ######################################################################

class TestSet:

	def __init__(self, name, number_of_cases, expected_range):
		self.name = name
		self.number_of_cases = number_of_cases
		self.expected_range = expected_range # ParamRange Object
		self.ranges = [] # list of ParamRanges

	def add_param_range(self, param):
		self.ranges.append(param)

	def clear_params(self):
		self.ranges.clear()

	def __str__(self):
		output = self.name+' ('+str(self.number_of_cases)+' test cases)\n'
		output += 'Expected Output Range ('+self.expected_range.param.type_name+'): '
		output += self.expected_range.v1+' '+self.expected_range.v2+' '+self.expected_range.v3+'\n'
		c = 1
		for x in self.ranges:
			output += 'Param ' + str(c) + ': ' + x.__str__() + '\n'
			c += 1
		return output

# NEW CLASS ######################################################################

class Method:

	def __init__(self, name='', class_name='', package_name='', output_type=''):
		self.name = name
		self.class_name = class_name
		self.package_name = package_name
		self.output_type = output_type
		self.params = [] # list of Parameters
		self.testsets = [] # list of TestSet

	def __str__(self):
		output = ''
		
		if (self.package_name == ''):
			output += "METHOD: "+self.name + "\nCLASS: " +self.class_name + "\nOUTPUT: " +self.output_type
		else:
			output += "METHOD: "+self.name + "\nCLASS: " +self.class_name + "\nPACKAGE: " +self.package_name + "\nOUTPUT: " +self.output_type
		
		c = 1
		for x in self.params:
			output += '\nPARAMETER '+str(c)+': '+x.name+' '+x.type_name
			c += 1
		
		c = 1
		for x in self.testsets:
			output += '\n\nEQUIVALENCE CLASS '+str(c)+': ' + x.__str__()
			c += 1
		
		return output

	def add_param_by_arg(self, param_name, type_name=''):
		param = Parameter(param_name,type_name)
		self.params.append(param)

	def add_param_by_parameter(self, param):
		self.params.append(param)	

	def remove_last_param(self):
		self.params.pop()		

	def add_testset(self, test):
		#t = TestSet(name, num)
		self.testsets.append(test)

	def remove_from_test_set_list(self, element):
		for x in range(0,len(self.testsets)):
			if (element == self.testsets[x].name):
				self.testsets.remove( self.testsets[x] )
				break