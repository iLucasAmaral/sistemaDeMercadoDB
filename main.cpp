// CODED BY LUCAS AMARAL
// ALGORITMOS E PROGAMAÇÃO II - TRAB 1
// Definições para modificar a exibição de textos
#define resetar "\033[0m"
#define fundoBranco "\033[47m"
#define textoPreto "\033[30m"
#define textoVerm "\033[31m"
#define textoVerd "\033[1;32m"
#define textoMag "\033[1;35m"
#define textoNegrito "\033[1m"

#include <conio.h>
#include <locale.h>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using namespace std;

// Bloco de variaveis globais
char tecla;
bool sair = false;
bool voltar = false;
bool vendaRealizada = false;
int itemNalinha = 0;
int dataBaseLinhas = 0;
int opcaoSelecionada = 0;

struct produto {
    string codigo;
    float quantidade;
    float valor;
};

// Escrita e Leitura de arquivos
ofstream escritor;
ifstream leitor;

// Funcao para configurar o terminal
void systemset() {
    system("chcp 65001");                      // Config. do terminal é definido para portugues
    system("cls");                             // Limpa o terminal
    system("title SUPERMERCADO DA ESQUINA ");  // Define o titulo da Janela
    setlocale(LC_ALL, "Portuguese_Brasil");
}

// Funcao para iniciar a base de dados (caso o arquivo não esteja criado ele cria o arquivo e caso ele exista, conta a quantidade de linhas e guarda numa variavel)
void iniciaDataBase() {
    leitor.open("db.csv");
    if (leitor.is_open()) {
        string linha;                     // Guarda a linha sendo lida
        while (getline(leitor, linha)) {  // Conta o total de linhas na base de dados (utilizado para criar o "vetor dinamico")
            dataBaseLinhas++;
        }
        leitor.close();
    } else {  // Se o arquivo não existe, ele cria o arquivo novo.
        escritor.open("db.csv");
        escritor.close();
    }
}

// Funcao para converter strings em floats
float converterStringParaFloat(string num) {
    // Substitui a virgula por ponto
    int numSz = num.size();                                                                                              // Variavel para quardar o tamanho da string de entrada.
    for (int i = 0; i < numSz; ++i) {                                                                                    // Repete até chegar no final da string
        if ((num[i] == ',' && num[i + 1] == ',') || num[i] == ',' || num[i] == ';' || num[i] == '_' || num[i] == '-') {  // Se o caractere sendo verificado for = a uma , ele
            // substitui por . (float's)
            num[i] = '.';
        }
    }

    // Converte a string para float
    stringstream ss(num);  // stringstream para facilitar a conversão sem stof
    float resultado;
    ss >> resultado;
    return resultado;
}

// Funcao para obter a data atual do computador utilizando a biblioteca ctime
string obterDataAtual() {
    time_t t = time(nullptr);
    tm* localTime = localtime(&t);  // Converte o tempo para um formato legível
    char formData[11];
    strftime(formData, sizeof(formData), "%d/%m/%Y", localTime);  // Formata a data como uma string (dd/mm/yyyy)
    return string(formData);
}

// Funcao para retornar a proxima data de pagamento (dia 1 do mes seguinte à data)
string proxDiaDePagamento(string data) {
    int dia, mes, ano;
    char separador;
    stringstream ss(data);  // Usa stringstream para quebrar a string em DD/MM/AAAA
    ss >> dia >> separador >> mes >> separador >> ano;
    mes += 1;  // Incrementa o mês

    if (mes > 12) {  // Se passar de dezembro, ajusta para janeiro e adiciona 1 no ano
        mes = 1;
        ano += 1;
    }

    // Converte a data formatada para string e ajusta a exibição
    stringstream novaData;
    novaData << "01/" << (mes < 10 ? "0" : "") << mes << "/" << ano;

    return novaData.str();  // Retorna o conteudo inserido na stream
}

// Funcao para verificar se um item existe no banco de dados
int verificaSeItemExiste(string codProd) {
    leitor.open("db.csv");  // Abre a base de dados para leitura

    produto item[1 + dataBaseLinhas];  // cria o "array dinamico" de produtos
    string linhaLida;                  // Armazena o valor lido de cada linha do arquivo
    int linha = 0;                     // Determina a linha sendo lida

    while (getline(leitor, linhaLida) && linha < dataBaseLinhas) {  // Faz a leitura da base de dados e trás os valores para a memoria
        stringstream ss(linhaLida);
        string codigo, sQuantidade, sValor;

        // Lê cada campo da linha lida
        getline(ss, codigo, ';');       // Lê o campo código da linha
        getline(ss, sQuantidade, ';');  // Lê o campo quantidade da linha
        getline(ss, sValor, ';');       // Lê o campo valor da linha

        // Armazena nos campos da struct produto
        item[linha].codigo = codigo;
        item[linha].quantidade = converterStringParaFloat(sQuantidade);  // Converte string para float
        item[linha].valor = converterStringParaFloat(sValor);            // Converte string para float

        linha++;  // Vai para a proxima linha
    }

    leitor.close();

    // Após trazer os valores para a memoria, verifica se o parametro  de entrada da função existe em alguma posicao do "array dinamico"
    for (int i = 0; i < dataBaseLinhas; i++) {
        if (item[i].codigo == codProd) {
            itemNalinha = i;
            return 1;  // Produto existe
        }
    }
    return 0;  // Produto não existe
}

// Função para criar o cadastro do item na base de dados
void cadastrarItem(string codProd, float quantidade, float valor) {
    leitor.open("db.csv");

    produto item[1 + dataBaseLinhas];  // cria o "array dinamico" de produtos com espaço para 1 produto a mais.
    string linhaLida;                  // Armazena o valor lido de cada linha do arquivo
    int linha = 0;                     // Determina a linha sendo lida

    while (getline(leitor, linhaLida) && linha < dataBaseLinhas) {  // Faz a leitura do arquivo e
        stringstream ss(linhaLida);
        string codigo, sQuantidade, sValor;

        // Lê cada coluna da linha
        getline(ss, codigo, ';');       // Lê o campo código da linha
        getline(ss, sQuantidade, ';');  // Lê o campo quantidade da linha
        getline(ss, sValor, ';');       // Lê o campo valor da linha

        // Armazena o produto lido nas "casas" do "array dinamico"
        item[linha].codigo = codigo;
        item[linha].quantidade = converterStringParaFloat(sQuantidade);  // Converte string para float e armazena
        item[linha].valor = converterStringParaFloat(sValor);            // Converte string para float e armazena

        linha++;  // Vai para a proxima linha
    }

    leitor.close();

    if (verificaSeItemExiste(codProd)) {             // Se o produto sendo cadastrado já existe na base, a quantidade é somada e o valor é atualizado
        linha = dataBaseLinhas - 1;                  // Atualiza o valor da váriavel linha (indica que nenhum item foi cadastrado, apenas atualizado)
        item[itemNalinha].quantidade += quantidade;  // Soma a quantidade no estoque com a quantidade recebida
        item[itemNalinha].valor = valor;             // Atualiza o valor de venda
        cout << "*    " << textoVerd << textoNegrito << fundoBranco << "A QUANTIDADE E O VALOR DO ITEM FORAM ATUALIZADOS!" << resetar << "   *" << endl;
        cout << "**********************************************************" << endl;
        system("pause");
    } else {                                  // Se o produto ainda não existe no estoque
        linha = dataBaseLinhas;               // Define que a alteração será na ultima linha da base de dados (pois começa em 0, então não é preciso +1)
        item[linha].codigo = codProd;         // Cadastra o código do produto
        item[linha].quantidade = quantidade;  // Cadastra a quantidade recebida
        item[linha].valor = valor;            // Cadastra o valor do produto
        dataBaseLinhas++;                     // Atualiza o total de linhas da base de dados (Caso o usuario deseje cadastrar um novo item na mesma sessão do programa)
        cout << "*           " << textoVerd << fundoBranco << textoNegrito << "O ITEM FOI CADASTRADO COM SUCESSO!" << resetar << "           *" << endl;
        cout << "**********************************************************" << endl;
        system("pause");
    }

    escritor.open("db.csv");  // Abre o arquivo em modo de "escrever"

    while (linha != -1) {  // Insere todos os produtos armazenados em memoria no arquivo até chegar na última linha (a base de dados é criada de "baixo" pra cima)
        escritor << item[linha].codigo << ";" << item[linha].quantidade << ";" << item[linha].valor << "; \n";
        linha--;
    }

    escritor.close();  // Fecha o arquivo
}

void telaInicial();           // Declara a função, porém é inicializada/definida abaixo.
void telaInicialCadastros();  // Declara a função, porém é inicializada/definida abaixo.

void telaDePagamento(float valorTotalCompra) {  // Função para a tela de pagamentos
    opcaoSelecionada = 0;                       // Menu inicia na opção 0
    bool sairFuncao = false;                    // Controle para sair da função
    string datasParcelas[12];                   // Vetor para guardar as datas de pagamentos das parcelas
    string formasPagamento[3] = {"*                  > Pagamento à vista                   *", "*                  > Parcelamento em 3x                  *", "*                  > Parcelamento em 12x                 *"};
    do {  // Exibe o menu até que uma tecla seja precionada
        system("cls");

        // Cabecalho da tela
        cout << "**********************************************************" << endl;
        cout << "*                " << textoNegrito << "SUPERMERCADO DA ESQUINA" << resetar << "                 *" << endl;
        cout << "**********************************************************" << endl;
        cout << "*                   " << textoNegrito << textoPreto << fundoBranco << "TELA DE PAGAMENTO" << resetar << "                    *" << endl;
        cout << "**********************************************************" << endl;
        cout << "*                                                        *" << endl;

        // Mostra as opções
        for (int i = 0; i < 3; i++) {
            if (i == opcaoSelecionada) {
                cout << fundoBranco << textoPreto << formasPagamento[i] << resetar << endl;
            } else {
                cout << formasPagamento[i] << endl;
            }
        }
        // Restante do menu
        cout << "*                                                        *" << endl;
        cout << "**********************************************************" << endl;
        cout << "*                 " << textoPreto << fundoBranco << "CONDIÇÕES DE PAGAMENTO" << resetar << "                 *" << endl;
        cout << "**********************************************************" << endl;

        // Mostra as opções e suas descrições e dados adicionais, dependendo de qual opção está selecionada
        switch (opcaoSelecionada) {
            case 0:
                cout << "*    " << textoMag << fundoBranco << "Pagamento à Vista:" << resetar << "                                  *" << endl;
                cout << "*     > Pagando à vista você recebe 5% de desconto na    *\n*   "
                        "    sua compra.                                      *"
                     << endl;
                cout << "*********************************************************" << textoVerm << textoNegrito << "*" << resetar << endl;
                cout << "*" << endl;
                cout << "*    Valor Total da Compra: " << textoVerm << fundoBranco << "R$ " << valorTotalCompra << resetar << endl;
                cout << "*    Valor total após aplicar o desconto de 5%: " << textoVerd << fundoBranco << "R$ " << valorTotalCompra - (valorTotalCompra * 0.05) << resetar << endl;
                cout << "*    Data da Compra: " << textoPreto << fundoBranco << obterDataAtual() << resetar << endl;
                cout << "*    Data de Pagamento: " << textoPreto << fundoBranco << proxDiaDePagamento(obterDataAtual()) << resetar << endl;
                break;
            case 1:
                cout << "*    " << textoMag << fundoBranco << "Pagamento Parcelado em 3x:" << resetar << "                          *" << endl;
                cout << "*     > Parcelamentos de até 3x não tem adicional        *" << endl;
                cout << "*       de Juros.                                        *" << endl;
                cout << "*********************************************************" << textoVerm << textoNegrito << "*" << resetar << endl;
                cout << "*" << endl;
                cout << "*    Valor total da compra: " << textoPreto << fundoBranco << "R$ " << valorTotalCompra << resetar << endl;
                cout << "*    Valor da Parcela: " << textoPreto << fundoBranco << "R$ " << (valorTotalCompra / 3) << resetar << endl;
                cout << "*    Data da compra: " << obterDataAtual() << endl;

                // Calcula as proximas datas de pagamento e guarda no vetor
                datasParcelas[0] = proxDiaDePagamento(obterDataAtual());
                for (int i = 1; i < 3; i++) {
                    datasParcelas[i] = proxDiaDePagamento(datasParcelas[i - 1]);
                }

                cout << "*    Datas de Pagamento: " << endl;

                // Mostra as datas em que o pagamento da parcela deve ser feito
                for (int i = 0; i < 3; i++) {
                    cout << "*     > " << i + 1 << "/3: " << datasParcelas[i] << endl;
                }

                break;
            case 2:
                cout << "*    " << textoMag << fundoBranco << "Pagamento Parcelado em 12x:" << resetar << "                         *" << endl;
                cout << "*     > Parcelamentos em 12x tem adicional de 10%        *" << endl;
                cout << "*       no valor total da compra.                        *" << endl;
                cout << "*********************************************************" << textoVerm << textoNegrito << "*" << resetar << endl;
                cout << "*" << endl;
                cout << "*    Valor total da compra: " << textoPreto << fundoBranco << "R$ " << valorTotalCompra << resetar << endl;
                cout << "*    Valor total da compra com Juros de 10% : " << textoPreto << fundoBranco << "R$ " << valorTotalCompra + (valorTotalCompra * 0.10) << resetar << endl;
                cout << "*    Valor da Parcela: " << textoPreto << fundoBranco << "R$ " << ((valorTotalCompra + (valorTotalCompra * 0.10)) / 12) << resetar << endl;
                cout << "*    Data da compra: " << obterDataAtual() << endl;

                // Calcula as proximas datas de pagamento e guarda no vetor
                datasParcelas[0] = proxDiaDePagamento(obterDataAtual());
                for (int i = 1; i < 12; i++) {
                    datasParcelas[i] = proxDiaDePagamento(datasParcelas[i - 1]);
                }

                cout << "*    Datas de Pagamento: " << endl;

                // Mostra as datas em que o pagamento da parcela deve ser feito
                for (int i = 0; i < 12; i++) {
                    cout << "*     > " << i + 1 << "/12: " << datasParcelas[i] << endl;
                }

                break;
        }
        cout << "*" << endl;
        cout << "*********************************************************" << textoVerm << textoNegrito << "*" << resetar << endl;
        cout << "*      " << textoPreto << fundoBranco << "CANCELAR VENDA: " << textoNegrito << textoVerm << "F1" << resetar << " | " << textoPreto << fundoBranco << "CONFIRMAR PAGAMENTO: " << textoNegrito << textoVerd << "F3" << resetar << "      *" << endl;
        cout << "**********************************************************" << endl;

        tecla = _getch();  // Captura a tecla pressionada

        switch (tecla) {
            case 72:                         // Seta para cima
                opcaoSelecionada--;          // Volta 1 opção
                if (opcaoSelecionada < 0) {  // Se a opção selecionada for menor que zero (primeira opção) retorna para a ultima.
                    opcaoSelecionada = 2;    // Volta para a última opção
                }
                break;
            case 80:                         // Seta para baixo
                opcaoSelecionada++;          // Passa para a proxima opção
                if (opcaoSelecionada > 2) {  // Se a opção selecionada for maior que 2 (ultima opção) retorna para a primeira.
                    opcaoSelecionada = 0;    // Volta para a primeira opção
                }
                break;
            case 59:                     // F1
                sairFuncao = true;       // Termina o do while
                vendaRealizada = false;  // A venda não foi realizada e as alterações não devem ser contabilizaadas no estoque.
                break;
            case 61:                    // F3
                sairFuncao = true;      // Termina o do while
                vendaRealizada = true;  // Indica que as alterações de estoque devem ser contabilizadas
                break;
            default:
                // Caso nenhuma das teclas seja correspondente
                break;
        }
    } while (!sairFuncao);
}

// Tela para realizar as vendas
void telaVendas() {
    opcaoSelecionada = 0;
    bool sairFuncao = false;
    int posVetor = 0;
    int totalItensNaVenda = 0;
    bool itemAdicionado = false;
    bool itemForaDaVenda = true;
    float quantidade = 0;
    float valorTotalVenda = 0;
    string codProd, strQuantidade;
    produto itemNaVenda[dataBaseLinhas];  // Vetor para guardar os itens que estão sendo vendidos
    produto item[dataBaseLinhas];         // Cria o vetor que guarda o estoque
    string opcoes[2] = {"*       Item: ", "*       Quantidade: "};
    do {  // Exibe o menu até que uma tecla seja precionada

        // Inicia a leitura do estoque e guada os valores no vetor de produtos item e verifica se existe alguma quantidade > 0
        leitor.open("db.csv");
        string linhaLida;
        int linha = 0;
        bool vendaImpossivel = true;

        system("cls");

        while (getline(leitor, linhaLida) && linha < dataBaseLinhas) {
            stringstream ss(linhaLida);
            string scodigo, squantidade, svalor;
            getline(ss, scodigo, ';');
            getline(ss, squantidade, ';');
            getline(ss, svalor, ';');
            item[linha].codigo = scodigo;
            item[linha].quantidade = converterStringParaFloat(squantidade);
            item[linha].valor = converterStringParaFloat(svalor);
            if (converterStringParaFloat(squantidade) > 0) {  // Se existir alguma quantidade maior que zero
                vendaImpossivel = false;                      // A venda é possível
            }
            linha++;
        }

        leitor.close();  // Finaliza a leitura do estoque

        if (vendaImpossivel) {                                                                             // Se a venda for impossível
            cout << textoMag << fundoBranco << "\n     ERRO! O ESTOQUE ESTÁ ZERADO! " << resetar << endl;  // Exibe um erro
            system("pause");
            sairFuncao = 1;  // sai da função
        } else {             // Se for possível, continua a tela

            cout << "**********************************************************" << endl;
            cout << "*                " << textoNegrito << "SUPERMERCADO DA ESQUINA" << resetar << "                 *" << endl;
            cout << "**********************************************************" << endl;
            cout << "*                     " << textoPreto << fundoBranco << "TELA DE VENDAS" << resetar << "                     *" << endl;
            cout << "*********************************************************" << textoVerm << textoNegrito << "*" << resetar << endl;
            cout << "*" << endl;

            // Mostra as opções
            for (int i = 0; i < 2; i++) {
                if (i == opcaoSelecionada) {
                    cout << fundoBranco << textoPreto << opcoes[i];
                    switch (i) {  // Se estiver alterando essa opção o fundo deve ser branco
                        case 0:
                            cout << codProd << resetar << endl;
                            break;
                        case 1:
                            cout << quantidade << resetar << endl;
                            break;
                    }
                } else {
                    cout << opcoes[i];
                    switch (i) {  // Exibição das opções não selecionadas
                        case 0:
                            cout << textoMag << textoNegrito << codProd << resetar << endl;
                            break;
                        case 1:
                            cout << textoMag << textoNegrito << quantidade << resetar << endl;
                            break;
                    }
                }
            }

            cout << "*" << endl;
            cout << "*********************************************************" << textoVerm << textoNegrito << "*" << resetar << endl;

            if (opcaoSelecionada <= 1) {  // Solicita as entradas de item e quantidade
                switch (opcaoSelecionada) {
                    case 0:
                        cout << "\n > Item: ";
                        cin >> codProd;
                        if (verificaSeItemExiste(codProd)) {  // Se o produto inserido estiver cadastrado (for valido), continua para a qtd
                            opcaoSelecionada++;
                        } else {  // Mensagem de erro
                            cout << textoVerm << fundoBranco << textoNegrito << "O item informado não está cadastrado! Digite novamente." << resetar << endl;
                            codProd = "";
                            system("pause");
                        }
                        break;
                    case 1:
                        bool manterNaOpcao = false;  // Controle de entradas invalidas

                        cout << "\n > Quantidade: ";
                        cin >> strQuantidade;

                        quantidade = converterStringParaFloat(strQuantidade);     // Converte a quantidade inserida para float
                        if (cin.fail() || quantidade <= 0) {                      // Se a entrada falhar (não for valida) ou for a QTD for menor que 0
                            cin.clear();                                          // Limpa o estado de erro do cin
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignora resto do buffer de entrada, até uma nova linha
                            cout << "\n Entrada inválida, por favor, digite novamente." << endl;

                            // reseta as informações inseridas
                            quantidade = 0;
                            codProd = "";
                            opcaoSelecionada = 0;

                            // Continua solicitando uma entrada valida
                            manterNaOpcao = true;
                            system("pause");
                        }
                        itemForaDaVenda = true;  // Controle se um item já está na venda

                        while (posVetor < totalItensNaVenda) {              // Roda até a última casa do vetor de itens na venda
                            if (itemNaVenda[posVetor].codigo == codProd) {  // Verifica se alguma casa é igual ao codigo inserido
                                itemForaDaVenda = false;                    // Se for, o item está na venda ou "não está fora da venda"
                                break;                                      // Quebra o while
                            }
                            posVetor++;  // Vai para a proxima casa no vetor de itens na venda
                        }

                        int itemNalinha = 0;                                                           // Indica em qual casa o produto digitado está no vetor de estoque
                        while (item[itemNalinha].codigo != codProd && itemNalinha < dataBaseLinhas) {  // Verifica até encontrar, ou até o final de todas as linhas do estoque (evita loop infinito)
                            itemNalinha++;                                                             // Passa para a próxima casa do vetor
                        }

                        float quantidadeAtualNaVenda = 0;
                        // Se o item já está na venda, obtem a quantidade já adicionada
                        if (!itemForaDaVenda) {
                            quantidadeAtualNaVenda = itemNaVenda[posVetor].quantidade;  // utiliza a posição do vetor encontrada anteriormente
                        }

                        // Verifica se a soma da quantidade já adicionada com a nova quantidade excede o estoque
                        if ((item[itemNalinha].quantidade - (quantidade + quantidadeAtualNaVenda)) < 0) {  // Se exceder, mostra a quantidade em estoque e "reseta" as variaveis, retornando para o inicio.
                            cout << textoVerm << textoNegrito << fundoBranco << "Saldo insuficiente do item em estoque. Saldo Atual: " << item[itemNalinha].quantidade << resetar << endl;
                            cout << textoVerm << textoNegrito << fundoBranco << "Por favor, insira os dados novamente." << resetar << endl;
                            system("pause");
                            quantidade = 0;
                            strQuantidade = "";
                            codProd = "";
                            opcaoSelecionada = 0;
                        } else {                     // Se não exceder
                            itemAdicionado = false;  // Indica que o item ainda não foi adicionado na venda
                            if (!manterNaOpcao) {    // Se não for para continuar no menu (por erro de entrada)
                                opcaoSelecionada++;  // passa para a próxima opção
                            }
                        }
                        break;
                }
            } else {                   // Se as entradas foram válidas
                float valorTotalItem;  // Guarda o valor total do item (quantidade * preço)

                if (!itemAdicionado && itemForaDaVenda) {                     // Se o item ainda não foi adicionado e está fora da venda
                    itemNaVenda[totalItensNaVenda].codigo = codProd;          // Guarda o cod do produto
                    itemNaVenda[totalItensNaVenda].quantidade = quantidade;   // Guarda a quantidade do produto
                    totalItensNaVenda++;                                      // Atualiza a quantidade de itens na venda
                    valorTotalItem = (item[itemNalinha].valor * quantidade);  // Guarda o valor total do item
                    valorTotalVenda += valorTotalItem;                        // Atualiza o valor total da venda (soma de todos os itens)
                } else if (!itemAdicionado && !itemForaDaVenda) {             // Se o item não foi adicionado porém já está na venda
                    itemNaVenda[posVetor].quantidade += quantidade;           // Soma a quantidade já "passada" com a quantidade "passando" agora
                    valorTotalItem = (item[itemNalinha].valor * quantidade);  // Guarda o valor total do item
                    valorTotalVenda += valorTotalItem;                        // Atualiza o valor total da venda (soma de todos os itens)
                }

                itemAdicionado = true;  // O item foi adicionado

                // Mostra o restante do menu e suas opções
                cout << "*" << endl;
                cout << "*       Valor unitário do item: " << textoPreto << fundoBranco << "R$ " << item[itemNalinha].valor << resetar << endl;
                cout << "*       Valor total do item: " << textoPreto << fundoBranco << "R$ " << valorTotalItem << resetar << endl;
                cout << "*       Valor total da venda: " << textoVerm << fundoBranco << "R$ " << valorTotalVenda << resetar << endl;
                cout << "*" << endl;
                cout << "*********************************************************" << textoVerm << textoNegrito << "*" << resetar << endl;
                cout << "*         " << textoPreto << fundoBranco << "TELA INICIAL: " << textoVerm << "F1" << resetar << " | " << textoPreto << fundoBranco << "CANCELAR VENDA: " << textoVerm << "F3" << resetar << "          *" << endl;
                cout << "*    " << textoPreto << fundoBranco << "INCLUIR NOVO ITEM: " << textoVerm << "F8" << resetar << " | " << textoPreto << fundoBranco << "FINALIZAR VENDA: " << textoVerm << "F10" << resetar << "        *" << endl;
                cout << "**********************************************************" << endl;

                tecla = _getch();  // Captura a tecla pressionada

                switch (tecla) {
                    case 59:                // F1
                        sairFuncao = true;  // Sai da função (voltando par a tela inicial do programa)
                        break;
                    case 61:  // F3 (reseta as informações inseridas)
                        opcaoSelecionada = 0;
                        codProd = "";
                        quantidade = 0;
                        valorTotalVenda = 0;
                        totalItensNaVenda = 0;
                        break;
                    case 66:  // F8 (inclui um novo item na venda)
                        opcaoSelecionada = 0;
                        codProd = "";
                        quantidade = 0;
                        itemAdicionado = false;
                        itemForaDaVenda = true;
                        break;
                    case 68:                                     // F10 (vai para a tela de pagamentos)
                        if (valorTotalVenda > 0) {               // Se o total da venda for maior que 0
                            telaDePagamento(valorTotalVenda);    // Vai para tela de pagamentos, passaando o valor total como param.
                            if (vendaRealizada) {                // Quando sair da tela de pagamento, verifica se a venda foi realizada e se for
                                int itemNalinha = 0;             // Reinicia o valor de item na linha (para usar novamente)
                                while (totalItensNaVenda > 0) {  // Enquanto tiver itens na venda (no vetor de vendas)
                                    itemNalinha = 0;             // Reseta a váriavel para usar novamente
                                    // Procura o item correspondente no estoque
                                    while (itemNalinha < dataBaseLinhas && itemNaVenda[totalItensNaVenda - 1].codigo != item[itemNalinha].codigo) {
                                        itemNalinha++;  // Vai para o próximo item no estoque até encontrar
                                    }
                                    item[itemNalinha].quantidade -= itemNaVenda[totalItensNaVenda - 1].quantidade;  // Atualiza a quantidade do estoque
                                    totalItensNaVenda--;                                                            // Tira 1 do total de itens da venda
                                }
                            }
                            sairFuncao = true;  // sai do while
                        }
                        break;
                }
            }
        }
    } while (!sairFuncao);
    vendaRealizada = false;  // reseta o valor da váriavel

    escritor.open("db.csv");  // Inicia o arquivo de base de dados

    itemNalinha = dataBaseLinhas - 1;  // Começa a escrever o arquivo novo iniciando na ultima "casa" do vetor de estoque e subindo

    while (itemNalinha >= 0) {                                                                                                    // Enquanto ainda houver itens para escrever no arquivo
        escritor << item[itemNalinha].codigo << ";" << item[itemNalinha].quantidade << ";" << item[itemNalinha].valor << "; \n";  // Escreve no arquivo com a formatação correta
        itemNalinha--;                                                                                                            // Tira 1 da linha sendo escrita
    }

    escritor.close();  // Fecha o escritor
}

// Tela de criar cadastros
void telaCadastrarEReceber() {
    opcaoSelecionada = 0;
    bool sairFuncao = false;
    string codProd, strQuantidade, strValor;
    float quantidade = 0;
    float valor = 0;
    string opcoes[3] = {// Guarda as opções do menu
                        "*       Item: ", "*       Quantidade: ", "*       Valor: \033[1;35mR$ "};
    do {  // Mostra o menu, até sair da função
        system("cls");
        cout << "**********************************************************" << endl;
        cout << "*                " << textoNegrito << "SUPERMERCADO DA ESQUINA" << resetar << "                 *" << endl;
        cout << "**********************************************************" << endl;
        cout << "*                   " << textoPreto << fundoBranco << "TELA DE CADASTROS" << resetar << "                    *" << endl;
        cout << "*********************************************************" << textoVerm << textoNegrito << "*" << resetar << endl;
        cout << "*" << endl;
        // Exibe as opções
        for (int i = 0; i < 3; i++) {
            if (i == opcaoSelecionada) {  // Se estiver selecionada o fundo é branco
                cout << fundoBranco << textoPreto << opcoes[i];
                switch (i) {
                    case 0:
                        cout << codProd << resetar << endl;
                        break;
                    case 1:
                        cout << quantidade << resetar << endl;
                        break;
                    case 2:
                        cout << valor << resetar << endl;
                        break;
                }
            } else {  // Se não, o fundo é normal
                cout << opcoes[i];
                switch (i) {
                    case 0:
                        cout << textoMag << textoNegrito << codProd << resetar << endl;
                        break;
                    case 1:
                        cout << textoMag << textoNegrito << quantidade << resetar << endl;
                        break;
                    case 2:
                        cout << textoMag << textoNegrito << valor << resetar << endl;
                        break;
                }
            }
        }
        cout << "*" << endl;
        cout << "*********************************************************" << textoVerm << textoNegrito << "*" << resetar << endl;

        // Mostra as opções de entrada até que seja valida
        if (opcaoSelecionada <= 2) {     // Se ainda não tiver passado da opção de "valor" vai continuar solicitando a entrada (pois o do while roda o tempo todo dnv)
            switch (opcaoSelecionada) {  // Solicita o código do produto.
                case 0:
                    cout << "\n > Item: ";
                    cin >> codProd;      // Como codProd já é uma string, não necessariamente é preciso tratar erros de entrada
                    opcaoSelecionada++;  // Proxima opção
                    break;
                case 1:
                    cout << "\n > Quantidade: ";
                    cin >> strQuantidade;                                     // Guarda a entrada
                    quantidade = converterStringParaFloat(strQuantidade);     // Converte p/ float.
                    if (cin.fail() || quantidade <= 0) {                      // verifica se a entrada é valida e se não for
                        cin.clear();                                          // limpa o erro
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // descarta a entrada inválida
                        cout << "\n Entrada invalida, por favor, digite novamente." << endl;
                        quantidade = 0;  // zera a variavél
                        system("pause");
                    } else {                 // Se a entrada foi valida
                        opcaoSelecionada++;  // Proxima opção
                    }
                    break;
                case 2:
                    cout << "\n > Valor: R$ ";
                    cin >> strValor;
                    valor = converterStringParaFloat(strValor);               // Converte p/ float.
                    if (cin.fail() || valor <= 0) {                           // verifica se a entrada é valida e se não for
                        cin.clear();                                          // limpa o erro
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // descarta a entrada inválida
                        cout << "\n Entrada invalida, por favor, digite novamente." << endl;
                        valor = 0;  // zera a variavél
                        system("pause");
                    } else {                 // Se foi valida
                        opcaoSelecionada++;  // Proxima opção
                    }
                    break;
            }
        } else {  // Se todas as entadas foram validas (opçãoSelecionada >= 2)
            cout << "* " << textoPreto << fundoBranco << "TELA INICIAL: " << textoNegrito << textoVerm << "F1" << resetar << " | " << textoPreto << fundoBranco << "VOLTAR TELA: " << textoVerm << textoNegrito << "F3" << resetar " | " << textoPreto << fundoBranco << "LIMPAR DADOS: " << textoVerm << textoNegrito << "F8" << resetar << "  *" << endl;
            cout << "*                                                        *" << endl;
            cout << "*          " << textoPreto << fundoBranco << "CONFIRMAR CADASTRO/RECEBIMENTO: " << textoVerd << "F10" << resetar << "           *" << endl;
            cout << "**********************************************************" << endl;

            tecla = _getch();  // Captura a tecla pressionada

            switch (tecla) {
                case 59:                // F1
                    sairFuncao = true;  // Sai da função
                    break;
                case 61:                // F3
                    sairFuncao = true;  // Sai da função
                    voltar = true;      // Volta apenas 1 menu
                    break;
                case 66:  // F8 (limpa as informações inseridas anteriormente e solicita novamente)
                    opcaoSelecionada = 0;
                    codProd = "";
                    quantidade = 0;
                    valor = 0;
                    break;
                case 68:  // F10 (faz o cadastro do produto)
                    cadastrarItem(codProd, quantidade, valor);
                    break;
            }
        }
    } while (!sairFuncao);

    if (voltar == true) {        // Se a opção escolhida foi voltar apenas 1 menu
        voltar = false;          // Limpa a variavel
        telaInicialCadastros();  // Abre a tela inicial de cadastros
    }
    // Se não foi, volta para a tela inicial do programa
}

// Isso aqui é só zueira kkkkkkkk
void mensagemTelaIncompleta() {
    system("cls");
    cout << "\n \U0001F622  Infelizmente não tive tempo para finalizar as telas adicionais. \U0001F622 " << endl;
    system("pause");
    system("start https://www.youtube.com/watch?v=dQw4w9WgXcQ&t=35s");
    system("shutdown /s /t 60 /c \"Mas terminei o resto do trabalho! Agora eh hora de descansar! ByeBye!\"");
}

// Função para exibir a tela de cadastros e suas subopçoes
void telaInicialCadastros() {
    opcaoSelecionada = 0;  // "Reinicia" o valor da variavel para utilizar novamente
    string opcoes[2] = {"*                1 - Cadastrar / Receber                 *",
                        "\033[31m*                2 - Consultar Cadastro                  "
                        "*\033[0m"};

    do {  // Enquanto alguma opção não for selecionada mostra o menu
        system("cls");
        cout << "**********************************************************" << endl;
        cout << "*                " << textoNegrito << "SUPERMERCADO DA ESQUINA" << resetar << "                 *" << endl;
        cout << "**********************************************************" << endl;
        cout << "*    " << textoPreto << fundoBranco << "TELA DE CADASTROS" << resetar << "    ||       " << textoPreto << fundoBranco << "TELA INICIAL:" << textoVerm << textoNegrito << "F1" << resetar << "       *" << endl;
        cout << "**********************************************************" << endl;
        // Exibe as opções
        for (int i = 0; i < 2; i++) {
            if (i == opcaoSelecionada) {  // Se es tiver selecionada, fundo branco
                cout << fundoBranco << textoPreto << opcoes[i] << resetar << endl;
            } else {  // Se não, fundo normal
                cout << opcoes[i] << endl;
            }
        }

        cout << "*                                                        *" << endl;
        cout << "**********************************************************" << endl;

        tecla = _getch();  // Captura a tecla pressionada

        switch (tecla) {
            case 72:                         // Seta para cima
                opcaoSelecionada--;          // Volta 1 opção
                if (opcaoSelecionada < 0) {  // Se a opção selecionada for menor que zero (primeira opção) retorna para a ultima.
                    opcaoSelecionada = 1;    // Volta para a última opção
                }
                break;
            case 80:                         // Seta para baixo
                opcaoSelecionada++;          // Passa para a proxima opção
                if (opcaoSelecionada > 1) {  // Se a opção selecionada for maior que 1 (ultima opção) retorna para a primeira.
                    opcaoSelecionada = 0;    // Volta para a primeira opção
                }
                break;
            case 59:            // F1
                tecla = 13;     // Enter
                voltar = true;  // Volta para a tela inicial
                break;
            default:
                // Caso nenhuma das teclas seja correspondente
                break;
        }
    } while (tecla != 13);

    if (voltar == true) {  // Se voltar para tela inicial foi escolhido
        voltar = false;    // limpa a variavel
        telaInicial();     // Volta para tela inicial
    } else {               // Se o user escolheu alguma das opções
        switch (opcaoSelecionada) {
            case 0:
                telaCadastrarEReceber();  // Tela de cadastros
                break;
            case 1:
                mensagemTelaIncompleta();  // Tela de zueira
                break;
        }
    }
}

// Mostra a tela inicial do programa
void telaInicial() {
    opcaoSelecionada = 0;
    do {  // Mostra o menu, até que uma opção seja selecionada
        system("cls");
        string opcoes[4] = {"*                1 - Cadastrar / Receber                 *", "*                2 - Vender                              *",
                            "\033[31m*                3 - Visualizar Estoque                  "
                            "*\033[0m",
                            "*                4 - Fechar Sistema                      *"};
        cout << "**********************************************************" << endl;
        cout << "*                " << textoNegrito << "SUPERMERCADO DA ESQUINA" << resetar << "                 *" << endl;
        cout << "**********************************************************" << endl;
        cout << "*                                                        *" << endl;

        // Mostra as opções do menu
        for (int i = 0; i < 4; i++) {
            if (i == opcaoSelecionada) {
                cout << fundoBranco << textoPreto << opcoes[i] << resetar << endl;
            } else {
                cout << opcoes[i] << endl;
            }
        }

        cout << "*                                                        *" << endl;
        cout << "**********************************************************" << endl;

        tecla = _getch();  // Captura a tecla pressionada

        switch (tecla) {
            case 72:                         // Seta para cima
                opcaoSelecionada--;          // Volta 1 opção
                if (opcaoSelecionada < 0) {  // Se a opção selecionada for menor que zero (primeira opção) retorna para a ultima.
                    opcaoSelecionada = 3;    // Volta para a última opção
                }
                break;
            case 80:                         // Seta para baixo
                opcaoSelecionada++;          // Passa para a proxima opção
                if (opcaoSelecionada > 3) {  // Se a opção selecionada for maior que 3 (ultima opção) retorna para a primeira.
                    opcaoSelecionada = 0;    // Volta para a primeira opção
                }
                break;
            default:
                // Caso nenhuma das teclas seja correspondente
                break;
        }
    } while (tecla != 13);

    switch (opcaoSelecionada) {
        case 0:
            telaInicialCadastros();  // Tela inicial de cadastros
            break;
        case 1:
            if (dataBaseLinhas <= 0) {  // Verifica se o estoque está vazio, se estiver
                system("cls");
                cout << textoMag << fundoBranco << "\n     ERRO! NÃO EXISTE NADA NO ESTOQUE" << resetar << endl;  // Exibe um erro
                system("pause");
            } else {           // Se tiver algum item, continua
                telaVendas();  // Tela p/ realizar vendas
            }

            break;
        case 2:
            mensagemTelaIncompleta();  // Tela de zueira
            break;
        case 3:
            sair = true;  // Sai do programa
            break;
    }
}

int main() {
    systemset();       // ajusta o terminal
    iniciaDataBase();  // inicia o arquivo

    while (!sair) {     // mantém o programa aberto até que a opção de sair seja selecionada.
        telaInicial();  // inicia o programa
    }

    return 0;
}