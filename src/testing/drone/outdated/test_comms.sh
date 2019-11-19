#!/bin/bash
#make -C ../argos3/build-hardware || exit 1
#sudo make install -C ../argos3/build-hardware || exit 1
make -C build || exit 1

tmux new-session -d -s instance "gdb -silent -ex='run' --args build/plugins/robots/drone/argos3 -c src/testing/test_drone.argos"
tmux rename-window "Test Drone"
tmux split-window -h "gdb -silent -ex='run' --args build/plugins/robots/drone/argos3 -c src/testing/test_drone.argos"
tmux split-window -h "gdb -silent -ex='run' --args build/plugins/robots/drone/argos3 -c src/testing/test_drone.argos"
tmux split-window -h "gdb -silent -ex='run' --args build/plugins/robots/drone/argos3 -c src/testing/test_drone.argos"
tmux split-window -h "gdb -silent -ex='run' --args build/plugins/robots/drone/argos3 -c src/testing/test_drone.argos"

tmux select-window -t instance:0
tmux select-pane -t 0

#tmux setw synchronize-panes
#tmux send-keys "" Enter
#tmux setw synchronize-panes off
tmux unbind-key C-x
tmux bind-key -n C-x setw synchronize-panes on\\\; send-keys "quit" Enter
tmux select-layout -t instance even-horizontal
tmux attach-session -t instance

# add key bindings to session

