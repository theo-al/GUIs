CMD := $(firstword $(MAKECMDGOALS))
ifeq ($(CMD),do)

EXD := $(word 2, $(MAKECMDGOALS))
ARQ := $(word 3, $(MAKECMDGOALS))

$(EXD):; @:
$(ARQ):; @:
do:
	@if [ -z "$(EXD)" ] || [ -z "$(ARQ)" ]; then \
	    echo "Uso: make do <ex> <ar>"; exit 1; \
	fi
	@$(MAKE) ex/$(EXD)/$(ARQ).out ex/$(EXD)/$(ARQ).gif

.PHONY: do
endif
