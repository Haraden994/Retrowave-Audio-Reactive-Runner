.PHONY: clean All

All:
	@echo "----------Building project:[ Retrowave -  ]----------"
	@cd "lectures_final\lecture06" && "$(MAKE)" -f  "Retrowave.mk" && "$(MAKE)" -f  "Retrowave.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ Retrowave -  ]----------"
	@cd "lectures_final\lecture06" && "$(MAKE)" -f  "Retrowave.mk" clean
