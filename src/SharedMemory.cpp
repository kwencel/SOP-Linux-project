#include "SharedMemory.h"
#include <algorithm>

int SharedMemory::selectSegmentFromVector(vector<Segment>* vectorPtr, string activity, bool extraChoice) {
    if (activity != "") {
        cout << "Choose one of the segments" << endl;
    }
    if (extraChoice) {
        cout << "0: I want to " << activity << " a segment created earlier" << endl;
    }
    for (int i = 0; i < (*vectorPtr).size(); ++i) {
        cout << i + 1 << ": ID: " << (*vectorPtr)[i].id << " Size: " << (*vectorPtr)[i].size << "B" << endl;
    }
    if (activity == "") {
        return 0;
    }
    int id;
    int choice;
    cin >> choice;
    if (extraChoice && choice == 0) {
        cout << endl;
        cout << "Please enter the segment id you want to " << activity << ":" << endl;
        cin >> id;
        return -id;
    } else {
        return choice - 1;
    } // if return value is negative - it's ID, if it's positive - it's an index of a vector
}

int SharedMemory::create(size_t size, int flags, key_t key) {
    int id = shmget(key, size, flags);
    if (id == -1) {
        perror("Can't create shared memory segment");
        return -1;
    }
    Segment segment = {id, size, nullptr};
    allSegments.push_back(segment);
    return id;
}

char* SharedMemory::attach() {
    int retval = selectSegmentFromVector(&allSegments, "attach", true);
    int id;
    size_t size;
    if (retval < 0) {
        cout << "Please enter the size of attaching segment:" << endl;
        cin >> size;
        id = -retval;
    } else {
        id = allSegments[retval].id;
    }
    char* ptr = (char*) shmat(id, NULL, 0);
    if ((intptr_t) ptr == -1) {
        perror("Can't attach the shared memory segment");
        return nullptr;
    }
    if (retval < 0) {
        Segment newSegment{id, size, ptr};
        allSegments.push_back(newSegment);
        attachedSegments.push_back(newSegment);
    } else {
        attachedSegments.push_back(allSegments[retval]);
        attachedSegments.back().ptr = ptr;
    }
    cout << "Shared memory segment attached successfully!" << endl;
    return ptr;
}

int SharedMemory::detach() {
    if (attachedSegments.empty()) {
        cout << "Nothing to detach." << endl;
        return 1;
    }
    int index = selectSegmentFromVector(&attachedSegments, "detach", false);
    attachedSegments.erase(attachedSegments.begin() + index);
    return shmdt(attachedSegments[index].ptr);
}

int SharedMemory::remove() {
    int retval = selectSegmentFromVector(&allSegments, "remove", true);
    if (retval < 0) {
        if (shmctl(-retval, IPC_RMID, NULL) != 0) {
            perror("Can't delete the shared memory segment");
            return -1;
        } else {
            cout << "Segment removed successfully!" << endl;
            return 0;
        }
    }
    int id = allSegments[retval].id;
    if (shmctl(id, IPC_RMID, NULL) != 0) {
        perror("Can't delete the shared memory segment");
        return -1;
    }
    // Determine if segment is currently attached (using lambda expression)
    // Remove the segment from attached vector if it was found there
    auto it = remove_if(attachedSegments.begin(), attachedSegments.end(), [id](Segment& seg) { return seg.id == id; });
    attachedSegments.erase(it, attachedSegments.end());
    // Remove the segment from the main vector
    allSegments.erase(allSegments.begin() + retval);
    cout << "Segment removed successfully!" << endl;
    return 0;
}

int SharedMemory::read() {
    if (attachedSegments.empty()) {
        cout << "Attach a memory segment first!" << endl;
        return 1;
    }
    int index = selectSegmentFromVector(&attachedSegments, "read", false);
    Segment segment = attachedSegments[index];
    cout << "Shared memory content:" << endl;
    for (int i = 0; i < segment.size; ++i) {
        cout << segment.ptr[i];
    }
    cout << endl;
    return 0;
}

int SharedMemory::write(string text) {
    if (attachedSegments.empty()) {
        cout << "Attach a memory segment first!" << endl;
        return 1;
    }
    int index = selectSegmentFromVector(&attachedSegments, "read", false);
    Segment segment = attachedSegments[index];
    int length = (int) text.length();
    if (length > segment.size) {
        cerr << "The text you entered won't fit fully into the shared memory segment!" << endl;
    }
    for (int i = 0; i <= segment.size; ++i) {
        segment.ptr[i] = text[i];
    }
    cout << "Written " << length << " bytes." << endl;
    return 0;
}

void SharedMemory::viewSegmentList() {
    cout << "All shared memory segments created in this session:" << endl;
    selectSegmentFromVector(&allSegments);
    cout << "Attached shared memory segments in this session:" << endl;
    selectSegmentFromVector(&attachedSegments);
}
