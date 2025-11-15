cmake_minimum_required(VERSION 3.10)

project(GuessTheNumber C)

add_executable(guess src/guess.c)

find_package(Gettext REQUIRED)

find_package(Intl REQUIRED)

if(GETTEXT_FOUND)
    message(STATUS "Gettext found")

    set(GETTEXT_DOMAIN "guess")
    set(PO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/po")
    set(LOCALE_DIR "${CMAKE_CURRENT_BINARY_DIR}/locale")

    add_custom_command(
        OUTPUT ${PO_DIR}/${GETTEXT_DOMAIN}.pot
        COMMAND xgettext --keyword=_ --from-code=UTF-8 -o ${PO_DIR}/${GETTEXT_DOMAIN}.pot ../src/guess.c
        COMMENT "Generating POT file"
    )

    add_custom_target(pot ALL DEPENDS ${PO_DIR}/${GETTEXT_DOMAIN}.pot)

    set(LANGUAGES ru)

    foreach(LANG ${LANGUAGES})
        set(PO_FILE ${PO_DIR}/${LANG}.po)
        set(MO_DIR ${LOCALE_DIR}/${LANG}/LC_MESSAGES)
        set(MO_FILE ${MO_DIR}/${GETTEXT_DOMAIN}.mo)

        file(MAKE_DIRECTORY ${MO_DIR})

        add_custom_command(
            OUTPUT ${PO_FILE}.dummy
            COMMAND msgmerge --update ${PO_FILE} ${PO_DIR}/${GETTEXT_DOMAIN}.pot
            DEPENDS ${PO_DIR}/${GETTEXT_DOMAIN}.pot
            COMMENT "Updating ${LANG}.po"
            VERBATIM
        )

        add_custom_command(
            OUTPUT ${MO_FILE}
            COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${MO_FILE} ${PO_FILE}
            DEPENDS ${PO_FILE}
            COMMENT "Compiling ${LANG}.po to ${MO_FILE}"
        )

        add_custom_target(mo_${LANG} ALL DEPENDS ${MO_FILE} ${PO_FILE}.dummy)
        add_dependencies(guess mo_${LANG})
    endforeach()

    install(
        DIRECTORY ${LOCALE_DIR}/
        DESTINATION share/locale
    )

    target_include_directories(guess PRIVATE ${Intl_INCLUDE_DIRS})
    target_link_libraries(guess PRIVATE ${Intl_LIBRARIES})

    add_custom_target(clean_locale
        COMMAND ${CMAKE_COMMAND} -E remove -f ${PO_DIR}/${GETTEXT_DOMAIN}.pot
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${LOCALE_DIR}
        COMMENT "Cleaning generated locale files"
    )

else()
    message(WARNING "Gettext not found. Building without localization support.")
endif()