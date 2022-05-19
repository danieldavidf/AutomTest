import sys,PySimpleGUI as sg
sys.path.append('../AutomTest/assets')

from components import Method, TestSet, Parameter, ParamRange
from generator import *
import layout, procedures as p

sg.theme('SystemDefault1')

# MUT = Method Under Test
MUT = Method()
#MUT = p.testStartup()
home = sg.Window('AutomTest', layout.home, finalize=True, element_justification='center')
win2_active=False

home['B1'].update(disabled=True)
home['B3'].update(disabled=True)
home['B4'].update(disabled=True)
#home['B4'].update(disabled=False)

while (True):
	ev1, vals1 = home.Read()
	if ev1 is None:
		break

	# Botão 1 - View Input data
	if (ev1 == 'B1' and not(win2_active)):
		win2_active = True
		home.Hide()
		win2 = sg.Window('Input data', layout.newLayoutVisualizarDados(MUT), finalize=True, element_justification='center')
		
		while (True):
			ev2, vals2 = win2.Read()
			if (ev2 is None or ev2 == 'Back'):
				win2.Close()
				win2_active = False
				home.UnHide()
				break


	# Botão 2 - Definir Informações do Método
	elif (ev1 == 'B2' and not(win2_active)):
		win2_active = True
		home.Hide()
		win2 = sg.Window('Set method information', layout.newLayoutInfoMetodo(MUT), finalize=True, element_justification='center')
		win2['Tipo'].update(set_to_index=p.get_tipoSaida_index(MUT))

		while (True):
			ev2, vals2 = win2.Read()

			if (ev2 is None or ev2 == 'Cancel'):
				win2.Close()
				win2_active = False
				home.UnHide()
				break

			if (ev2 == 'Next'):
				if (vals2[0]!="" and vals2[2]!="" and vals2[3]!="" and vals2['Tipo']!=""):
					#MUT = Method(vals2[2], vals2[0], vals2[1], vals2['Tipo'])
					if ( not p.confere_entrada_params( vals2[3] ) ):
						sg.popup_quick_message('Parameters incorrectly filled in.')
					else:
						MUT.name = vals2[2]
						MUT.class_name = vals2[0]
						MUT.package_name = vals2[1]
						MUT.output_type = vals2['Tipo']
						all_params = []
						all_params.clear()
						MUT.params.clear()
						all_params = vals2[3].replace(" ", "").split(',')
						for x in all_params:
							MUT.add_param_by_arg(x)
						win2.Close()

						win2 = sg.Window('Inform types of parameters', layout.newLayoutTiposParams(MUT), finalize=True, element_justification='center')
						while (True):
							ev2, vals2 = win2.Read()

							if (ev2 is None or ev2 == 'Back'):
								win2.Close()
								win2_active = False
								home.UnHide()
								break

							if (ev2 == 'Next'):

								valida = True
								for x in range(0,len(vals2)):
									if (vals2[x] == ''):
										valida = False
										break

								if (valida):
									for x in range(0,len(MUT.params)):
										MUT.params[x].type_name = vals2[x]
									win2.Close()
									win2_active = False
									home.UnHide()
									home['B1'].update(disabled=False)
									home['B3'].update(disabled=False)
									#home['B2'].update(button_color=('white', 'green'))
									home['B2'].update('Method information defined ✓')
									# home['B3'].update(button_color=('white','#283b5b'))
									break
								else:
									sg.popup_quick_message('Please fill out all fields.')

						break
				
				else:
					sg.popup_quick_message('Please fill in all required fields.')


	# Botão 3 - Especificar Conjuntos de Teste
	if (ev1 == 'B3' and not(win2_active)):
		win2_active = True
		home.Hide()
		win2 = sg.Window('Specification of test suites', layout.newLayoutConjuntosTeste(MUT), finalize=True, element_justification='center')
		#disable_minimize=True,#disable_close=True,
		while (True):
			ev2, vals2 = win2.Read()

			if (ev2 is None or ev2 == 'Back'):
				win2.Close()
				win2_active = False
				home.UnHide()
				break

			elif (ev2 == 'Remove'):
				if (len(vals2[0]) > 0):
					MUT.remove_from_test_set_list( vals2[0][0] )
					win2.Close()
					home.Hide()
					if (len(MUT.testsets)==0):
						home['B4'].update(disabled=True)
						home['B3'].update('Specification of test suites')
					win2 = sg.Window('Specification of test suites', layout.newLayoutConjuntosTeste(MUT), finalize=True, element_justification='center')

			elif (ev2 == 'Params'): # Vai para janela de Especificar Parâmetros
				par = Parameter("expected_output", MUT.params[x].type_name)
				if (MUT.output_type == 'boolean' or MUT.output_type == 'char'):
					ppp = ParamRange(par,vals2[4])
					vals2[5] = ''
					vals2[6] = ''
				elif (MUT.output_type == 'String'):
					ppp = ParamRange(par,vals2[4],vals2[5])
					vals2[6] = ''
				else: # int / double / float / Date
					ppp = ParamRange(par,vals2[4],vals2[5],vals2[6])
					

				if ( p.telaInicialConjTesteCorreta(vals2[2],vals2[3]) and p.entradaTipoCorreta(MUT.output_type,vals2[4],vals2[5],vals2[6])):
					test = TestSet(vals2[2], int(vals2[3]),ppp)
					win3_active = True
					win2.Hide()
					win3 = sg.Window('Parameters of the new test suite', layout.newLayoutEspecificarParam(MUT, vals2[2]),
						finalize=True, element_justification='center')
					while (True):
						ev3, vals3 = win3.Read()

						if (ev3 is None or ev3 == 'Cancelar'):
							if ( len(MUT.testsets) > 0 ):
								MUT.testsets.pop()
							win3.Close()
							win2.UnHide()
							win3_active = False
							break

						elif (ev3 == 'Confirmar'):
							cont = 0
							valida = True
							for x in range(0,len(MUT.params)):
								if (MUT.params[x].type_name == 'boolean' or MUT.params[x].type_name == 'char'):
									if( p.entradaTipoCorreta(MUT.params[x], vals3[cont]) ):
										par = Parameter(MUT.params[x].name, MUT.params[x].type_name)
										pr = ParamRange(par, vals3[cont])
										test.add_param_range(pr)
										cont += 1
									else:
										test.clear_params()
										valida = False
										break
								elif (MUT.params[x].type_name == 'String'):
									if( p.entradaTipoCorreta(MUT.params[x], vals3[cont], vals3[cont+1]) ):
										par = Parameter(MUT.params[x].name, MUT.params[x].type_name)
										pr = ParamRange(par, vals3[cont], vals3[cont+1])
										test.add_param_range(pr)
										cont += 2
									else:
										test.clear_params()
										valida = False
										break
								else: # int / float / double / Date
									if( p.entradaTipoCorreta(MUT.params[x], vals3[cont], vals3[cont+1], vals3[cont+2]) ):
										par = Parameter(MUT.params[x].name, MUT.params[x].type_name)
										pr = ParamRange(par, vals3[cont], vals3[cont+1], vals3[cont+2])
										test.add_param_range(pr)
										cont += 3
									else:
										test.clear_params()
										valida = False
										break

							if (valida):
								MUT.add_testset(test)
								win3.Close()
								win3_active = False
								win2.Close()
								home.Hide()
								home['B4'].update(disabled=False)
								home['B4'].update(button_color=('white','#006400'))
								home['B3'].update('Specified test suites ✓')
								win2 = sg.Window('Specification of test suites', layout.newLayoutConjuntosTeste(MUT), finalize=True, element_justification='center')
								break


	# Botão 4 - GERAR TESTES
	# TO DO
	if (ev1 == 'B4' and not(win2_active)):
		generate_tests( MUT )
		break


	# Botão 5 - Sobre AutomTest
	if (ev1 == 'B5' and not(win2_active)):
		win2_active = True
		home.Hide()
		win2 = sg.Window('About AutomTest', layout.newLayoutSobre(), finalize=True, element_justification='center')
		#disable_minimize=True,#disable_close=True,
		while (True):
			ev2, vals2 = win2.Read()
			if (ev2 is None or ev2 == 'Back'):
				win2.Close()
				win2_active = False
				home.UnHide()
				break

print('\n******************** Data ********************\n')
print(MUT)
#sg.SystemTray.notify(MUT.class_name+'Test.java','Successfully generated tests.')
