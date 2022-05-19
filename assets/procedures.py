import os,io,sys,PySimpleGUI as sg
sys.path.append('../assets')
import webbrowser,random,datetime
from PIL import Image
#from components import Parameter, TestSet
from components import Method, TestSet, Parameter, ParamRange

def open(url):
    webbrowser.open_new(url)

def get_img_data(f, maxsize=(1200, 850), first=False):
    img = Image.open(f)
    img.thumbnail(maxsize)
    if first:
        bio = io.BytesIO()
        img.save(bio, format="PNG")
        del img
        return bio.getvalue()
    return ImageTk.PhotoImage(img)

def get_tipoSaida_index(MUT):
    if (MUT.output_type == 'int'):
    	return 0
    elif (MUT.output_type == 'String'):
    	return 1
    elif (MUT.output_type == 'float'):
    	return 2
    elif (MUT.output_type == 'double'):
    	return 3
    elif (MUT.output_type == 'char'):
    	return 4
    elif (MUT.output_type == 'boolean'):
    	return 5
    elif (MUT.output_type == 'Date'):
    	return 6

def isNumber(value):
    try:
         float(value)
    except ValueError:
         return False
    return True

def get_a_number(min_val,max_val):
    return random.randint(min_val, max_val)


def confere_entrada_params(s):
	if (s==''):
		return False
	if ( s[0]==',' or s[len(s)-1]==','):
		return False
	else:
		if (s.find(',,') != -1 or s.find('.') != -1):
			return False
		else:
			# verifica se nao tem parametros com o mesmo nome
			tmp = s.replace(" ", "").split(',')
			
			for x in range(0,len(tmp)-1):
				for y in range(x+1,len(tmp)):
					if (tmp[x] == tmp[y]):
						print(tmp[x] + ' equals '+tmp[y])
						return False
			return True

def confere_colchetes(s):
	abre = 0
	fecha = 0
	for x in s:
		if (x == '['):
			abre += 1
		elif (x == ']'):
			fecha += 1
	if (fecha > 0 and abre == fecha):
		return True
	else:
		return False

def testStartup():

	MUT = Method('isNumber', 'Cenario2', 'cenarios', 'boolean')
	param1 = Parameter('numero', 'String')
	param2 = Parameter('saida_esperada', 'boolean')
	MUT.add_param_by_parameter(param1) # saida esperada

	# CONJUNTO DE TESTE 1
	pr1 = ParamRange(param1, '[-][numeros][,][numeros]', '[0~1][1~3][0~1][1~3]', '')
	expected = ParamRange(param2, 'true')
	t1 = TestSet("entrada_valida", 500, expected)
	t1.add_param_range(pr1)
 
	# CONJUNTO DE TESTE 2
	pr1 = ParamRange(param1, '[,][-][numeros][,][numeros]', '[1][0~1][1~3][0~1][1~3]', '')
	expected = ParamRange(param2, 'false')
	t2 = TestSet("comeca_com_virgula", 50, expected)
	t2.add_param_range(pr1)

	MUT.add_testset( t1 )
	MUT.add_testset( t2 )
	return MUT

def telaInicialConjTesteCorreta(nome, num):
	if (nome!='' and num!=''):
		if ( not( isNumber(num) ) ):
			sg.popup_quick_message('The number of test cases was not filled in correctly.')
			return False
		elif ( isNumber( nome[0] ) ):
			sg.popup_quick_message('The name of the set should not start with a number.')
			return False
		else:
			return True
	else:
		sg.popup_quick_message('Please fill in the required fields.')
		return False

def isDate(strdate):

	if (strdate.find('/') != -1):
		i = strdate.split('/')
		y = int(i[0])
		m = int(i[1])
		d = int(i[2])
	elif(strdate.find('-') != -1):
		i = strdate.split('-')
		y = int(i[0])
		m = int(i[1])
		d = int(i[2])
	else:
		return False

	try:
	    newDate = datetime.datetime(y,m,d)
	    return True
	except ValueError:
	    return False

def entradaTipoCorreta(tipo, out1, out2='', out3=''):

	if ( isinstance(tipo,Parameter) ):
		nome = tipo.name
		tipo = tipo.type_name
		b=True
	else:
		b=False
		nome = ''
	
	if (tipo == 'String'):
		if (out1!='' and out2!=''):
			if (confere_colchetes(out1) and confere_colchetes(out2)):
				return True
			else:
				if (b):
					sg.popup_quick_message('Parameter range '+nome+':\nNo brackets or brackets do not check.')
				else:
					sg.popup_quick_message('Expected Output:\nNo brackets or brackets do not check.')
		else:
			if (b):
				sg.popup_quick_message('Parameter range '+nome+':\nPlease fill in the required fields.')
			else:
				sg.popup_quick_message('Expected Output:\nPlease fill in the required fields.')
			
	elif (tipo == 'char' and out1!=''):
		return True
	
	elif (tipo == 'boolean'):
		if (out1==''):
			if (b):
				sg.popup_quick_message('Parameter range '+nome+':\nPlease fill in the required fields.')
			else:
				sg.popup_quick_message('Expected Output:\nPlease fill in the required fields.')

		else:
			return True
	
	# int / float / double / Date
	elif ( (out1!='' and out2!='') or (out3!='' and out1=='' and out2=='') ):
		
		# Apenas DE - ATÉ
		if ( out3=='' and out1!='' and out2!=''):
			
			if (tipo == 'Date'):
				if (not( isDate(out1) ) or not( isDate(out2) ) ):
					if (b):
						sg.popup_quick_message('Parameter range '+nome+':\nThe range \"From - to\" should contain only Date.')
					else:
						sg.popup_quick_message('Expected Output:\nThe range \"From - to\" should contain only Date.')
					return False
				elif ( out1 > out2 ):
					if (b):
						sg.popup_quick_message('Parameter range '+nome+':\n\"From\" must be before than \"To\".')
					else:
						sg.popup_quick_message('Expected Output:\nThe range \"From\" must be before than \"To\".')
					return False
				else:
					return True
			
			else: #int/float/double
				if (not( isNumber(out1) ) or not( isNumber(out2) ) ):
					if (b):
						sg.popup_quick_message('Parameter range '+nome+':\nThe range \"From - to\" should contain only numbers.')
					else:
						sg.popup_quick_message('Expected Output:\nThe range \"From - to\" should contain only numbers.')
					return False
				elif (float(out1) >= float(out2)):
					if (b):
						sg.popup_quick_message('Parameter range '+nome+':\n\"From\" must be less than \"To\".')
					else:
						sg.popup_quick_message('Expected Output:\nThe range \"From\" must be less than \"To\".')
					return False
				else:
					return True
		
		# Apenas Incluir
		elif (out3!='' and out1=='' and out2==''):

			if(tipo == 'Date'):
				if ( isDate(out3) ):
					return True
				else:
					if (b):
						sg.popup_quick_message('Parameter range '+nome+':is not a Date type.')
					else:
						sg.popup_quick_message('Expected Output:\nIt is not a Date type.')
					return False
			else:
				s = out3.replace(" ", "")
				valor = s.split(',')
				valida = True
				for x in range(0,len(valor)):
					if ( not( isNumber( valor[x] ) ) ):
						valida = False
						break
				if (valida):
					return True
				else:
					sg.popup_quick_message('')
					if (b):
						sg.popup_quick_message('Parameter range '+nome+':\nExpected Output should contain only numbers.')
					else:
						sg.popup_quick_message('Expected Output:\nExpected Output should contain only numbers.')
					return False
		
		# DE - ATÉ e Incluir
		elif ( out1!='' and out2!='' and out3!='' ):
			
			if (tipo == 'Date'):
				if (isDate(out1) and isDate(out2) and isDate(out3)):
					if ( out1 > out2 ):
						if (b):
							sg.popup_quick_message('Parameter range '+nome+':\n\"From\" must be before than \"To\".')
						else:
							sg.popup_quick_message('Expected Output:\nThe range \"From\" must be before than \"To\".')
						return False
					else:
						return True
				else:
					if (b):
						sg.popup_quick_message('Parameter range '+nome+': Incorrect Date format.')
					else:
						sg.popup_quick_message('Expected Output: Incorrect Date format.')
					return False

			else:
				s = out3.replace(" ", "")
				valor = s.split(',')
				valida = True
				for x in range(0,len(valor)):
					if ( not( isNumber( valor[x] ) ) ):
						valida = False
						break
				if (valida):
					if (not( isNumber(out1) ) or not( isNumber(out2) ) ):
						if (b):
							sg.popup_quick_message('Parameter range '+nome+':\nThe range \"From__ to __\" should contain only numbers.')
						else:
							sg.popup_quick_message('Expected Output:\nThe range \"From__ to __\" should contain only numbers.')
						return False
					elif (float(out1) >= float(out2)):
						if (b):
							sg.popup_quick_message('Parameter range '+nome+':\n\"From\" ('+str(out1)+') must be less than \"To\" ('+str(out2)+').')
						else:
							sg.popup_quick_message('Expected Output:\n\"From\" ('+str(out1)+') must be less than \"To\" ('+str(out2)+').')
						return False
					else:
						return True
				else:
					if (b):
						sg.popup_quick_message('Parameter range:\n\'Include\' should contain only numbers separated by commas.')
					else:
						sg.popup_quick_message('Expected Output:\n\'Include\' should contain only numbers separated by commas.')
					return False
		else:
			if (b):
				sg.popup_quick_message('Parameter range '+nome+':\nPlease fill in the required fields.')
			else:
				sg.popup_quick_message('Expected Output:\nPlease fill in the required fields.')
			return False
	else:
		if (b):
			sg.popup_quick_message('Parameter range '+nome+':\nPlease fill in the required fields.')
		else:
			sg.popup_quick_message('Expected Output:\nPlease fill in the required fields.')
		return False

#oficial documentation: https://pysimplegui.readthedocs.io/en/latest/
#crtl+f ELEMENT AND FUNCTION CALL REFERENCE