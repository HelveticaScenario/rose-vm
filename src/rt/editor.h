#pragma once

struct rose_editor_instance {
    rose_rt* rt;
};

struct rose_editor {
    vector<rose_editor_instance*> editors;
};

