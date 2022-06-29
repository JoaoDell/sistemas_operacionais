#!/bin/bash

# João Victor Dell Agli Floriano - 10799783
# Sistemas Operacionais - 2022.1

command=$1

# Checando se o arquivo é executável
if [ ! -x "$command" ]; then
    chmod +x "$command"
fi

# Sendo agora executável, checando se a primeira linha começa com '#!'
# Se sim, ignora o if
# Se não, cria um backup, adiciona a linha '#!/bin/bash' e concatena o conteúdo do backup ao arquivo final 
if [ -z $(head -1 "$command" | grep ^'#!') ]; then
    cat "$command" > "$command".bak
    
    # parte de criação de arquivo
    echo '#!/bin/bash' > "$command"
    cat "$command".bak >> "$command"
fi