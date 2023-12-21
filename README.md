**Compte rendu TP2 par Constant BRISSIER et Audric OLIVIER**

## Question 1: Utilisation des Arguments de la Ligne de Commande

L'implémentation commence par la gestion des arguments passés en ligne de commande. Le programme contrôle la présence de trois arguments – son propre nom, suivi du nom du serveur et du nom de fichier. Ceci illustre la récupération et l'utilisation des paramètres nécessaires pour la suite des opérations de transfert TFTP. Une vérification est effectuée pour s'assurer que le nombre d'arguments reçus correspond à l'attente. Des informations d'usage sont affichées en cas de discordance. La fonction `print_server_info` démontre la capacité du programme à recevoir et manipuler les chaînes de caractères fournies par l'utilisateur.

## Question 2: Résolution de l'Adresse du Serveur

Grâce à la fonction `getaddrinfo`, le programme convertit le nom de domaine donné en paramètre en une adresse IP exploitable. Cela constitue une étape crucialle car elle permet de traduire un nom de domaine human-readable en une adresse technique requise pour la communication réseau. Le programme prend un nom de domaine en argument et utilise la fonction pour obtenir une liste d'adresses correspondantes en format numérique. La fonction `print_resolved_address` illustre la capacité du programme à itérer sur cette liste et à afficher la première adresse IP valide.

## Question 3: Création et Gestion de Socket

Le processus de création d'un socket est initié en appelant la fonction `create_socket` qui utilise les informations résolues précédemment. Cette étape est essentielle pour la mise en place d'une communication réseau. Le socket crée permet au client de se préparer pour l'envoi et la réception des données via le protocole TFTP. Ce procédé utilise des arguments importants comme la famille d'adresses (IPv4 ou IPv6) et le type de socket, qui dans le cas de TFTP est basé sur UDP. L'addressage et le port de connexion sont retirés de la structure renvoyée par `getaddrinfo`. La création et l'envoi d'une requête de lecture RRQ, conforme au standard TFTP, sont exécutés à l'aide de la fonction `send_tftp_rrq`, démontrant ainsi la capacité du programme à communiquer avec un serveur TFTP.

---
